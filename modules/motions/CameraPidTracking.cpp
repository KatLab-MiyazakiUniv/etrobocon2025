/**
 * @file   CameraPidTracking.cpp
 * @brief  カメラを使ったPID走行の親クラス
 * @author miyahara046 HaruArima08
 */

#include "CameraPidTracking.h"
#include <vector>

CameraPidTracking::CameraPidTracking(Robot& _robot, double _targetSpeed, int _targetXCoordinate,
                                     const PidGain& _pidGain,
                                     BoundingBoxDetector& _boundingBoxDetector)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetXCoordinate(_targetXCoordinate),
    pidGain(_pidGain),
    boundingBoxDetector(_boundingBoxDetector),
    recoveryValidator(80.0)
{
}

void CameraPidTracking::run()
{
  Pid pid(pidGain.kp, pidGain.ki, pidGain.kd, targetXCoordinate);
  // 事前条件を判定する
  if(!isMetPreCondition()) {
    return;
  }

  // 事前準備
  prepare();

  SpeedCalculator speedCalculator(robot, targetSpeed);

  // 継続条件を満たしている間ループ
  while(isMetContinuationCondition()) {
    // 初期Speed値を計算
    double baseRightPower = speedCalculator.calculateRightMotorPower();
    double baseLeftPower = speedCalculator.calculateLeftMotorPower();

    // カメラからフレームを取得
    cv::Mat frame;
    robot.getCameraCaptureInstance().getFrame(frame);

    // 画像処理を実行
    boundingBoxDetector.detect(frame, result);

    // 検出失敗時は復帰動作を実行
    if(!result.wasDetected) {
      if(performRecoveryAction()) {
        continue;  // 復帰成功で通常処理続行
      } else {
        break;  // 復帰失敗で停止
      }
    }

    // バウンディングボックスの中心X座標を計算
    double currentX = (result.topLeft.x + result.bottomRight.x) / 2.0;

    // 旋回値の計算
    double turningPower = pid.calculatePid(currentX);

    // モータのPower値をセット（前進の時0を下回らないように，後進の時0を上回らないようにセット）
    double rightPower = baseRightPower > 0.0 ? std::max(baseRightPower - turningPower, 0.0)
                                             : std::min(baseRightPower + turningPower, 0.0);
    double leftPower = baseLeftPower > 0.0 ? std::max(baseLeftPower + turningPower, 0.0)
                                           : std::min(baseLeftPower - turningPower, 0.0);
    robot.getMotorControllerInstance().setRightMotorPower(rightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(leftPower);

    // 10ms待機
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // モータを停止
  robot.getMotorControllerInstance().stopWheelsMotor();
}

bool CameraPidTracking::performRecoveryAction()
{
  // 復帰動作前の基準位置を保存
  recoveryValidator.savePreRecoveryPosition(targetXCoordinate);

  // 角度回頭による復帰を試行
  bool recoverySuccess = tryRecoveryRotation();

  // 復帰完了時は検証データをクリア
  recoveryValidator.clear();

  return recoverySuccess;
}

bool CameraPidTracking::tryRecoveryRotation()
{
  // 回頭角度リスト（左右対称で段階的に拡大）
  std::vector<int> rotationAngles = { 1, 2, 3 };

  for(int angle : rotationAngles) {
    // 左回頭で検出試行
    AngleRotation leftRotation(robot, angle, 50, false);
    leftRotation.run();

    // 短時間待機後に検出試行
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    cv::Mat frame;
    robot.getCameraCaptureInstance().getFrame(frame);
    boundingBoxDetector.detect(frame, result);

    if(result.wasDetected) {
      // 位置変化チェック
      double currentX = (result.topLeft.x + result.bottomRight.x) / 2.0;
      if(recoveryValidator.isPositionChangeValid(currentX)) {
        return true;  // 位置変化が中心X座標変化量の範囲内なら復帰成功
      }
    }

    // 元の位置に戻る (右回頭)
    AngleRotation returnRotation1(robot, angle, 100, true);
    returnRotation1.run();

    // さらに右回頭で検出試行
    AngleRotation rightRotation(robot, angle, 100, true);
    rightRotation.run();

    // 短時間待機後に検出試行
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    robot.getCameraCaptureInstance().getFrame(frame);
    boundingBoxDetector.detect(frame, result);

    if(result.wasDetected) {
      // 位置変化チェック
      double currentX = (result.topLeft.x + result.bottomRight.x) / 2.0;
      if(recoveryValidator.isPositionChangeValid(currentX)) {
        return true;  // 位置変化が中心X座標変化量の範囲内なら復帰成功
      }
    }

    // 元の位置に戻る（左回頭）
    AngleRotation returnRotation2(robot, angle, 100, false);
    returnRotation2.run();
  }

  return false;  // 全ての試行で検出失敗
}
