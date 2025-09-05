/**
 * @file   DualCameraPidTracking.cpp
 * @brief  カメラを使ったPID走行の親クラス
 * @author miyahara046 HaruArima08
 */

#include "DualCameraPidTracking.h"

DualCameraPidTracking::DualCameraPidTracking(Robot& _robot, double _targetSpeed,
                                             int _targetXCoordinate, int _targetXCoordinate2,
                                             const PidGain& _pidGain,
                                             DualLineBoundingBoxDetector& _boundingBoxDetector)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetXCoordinate(_targetXCoordinate),
    targetXCoordinate2(_targetXCoordinate2),
    pidGain(_pidGain),
    boundingBoxDetector(_boundingBoxDetector)
{
}

void DualCameraPidTracking::run()
{
  Pid pid(pidGain.kp, pidGain.ki, pidGain.kd, targetXCoordinate);
  Pid pid2(pidGain.kp, pidGain.ki, pidGain.kd, targetXCoordinate2);
  // 事前条件を判定する
  if(!isMetPreCondition()) {
    return;
  }

  // 事前準備
  prepare();

  // 左右で符号を変える
  int edgeSign = robot.getIsLeftEdge() ? -1 : 1;

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

    // 検出失敗時はスキップする
    if(!result.wasDetected) {
      continue;
    }

    // バウンディングボックスの中心X座標を計算
    double currentX = (result.topLeft.x + result.bottomRight.x) / 2.0;

    // 旋回値の計算
    double turningPower;
    if(boundingBoxDetector.getWasColorSwitched()) {
      turningPower = pid2.calculatePid(currentX) * edgeSign;
    } else {
      turningPower = pid.calculatePid(currentX) * edgeSign;
    }
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