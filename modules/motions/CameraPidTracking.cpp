/**
 * @file   CameraPidTracking.cpp
 * @brief  カメラ画像を使ったPIDライントレースの親クラス
 * @author miyahara046 HaruArima08
 */

#include "CameraPidTracking.h"

CameraPidTracking::CameraPidTracking(Robot& _robot, double _targetSpeed, int _targetPoint,
                                     const PidGain& _pidGain,
                                     BoundingBoxDetector& _boundingBoxDetector,
                                     CameraCapture& _cameraCapture)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetPoint(_targetPoint),
    pidGain(_pidGain),
    boundingBoxDetector(_boundingBoxDetector),
    cameraCapture(_cameraCapture)
{
}

void CameraPidTracking::run()
{
  Pid pid(pidGain.kp, pidGain.ki, pidGain.kd, targetPoint);
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
    cameraCapture.getFrame(frame);

    // 画像処理を実行
    boundingBoxDetector.detect(frame, result);

    // バウンディングボックスの中心X座標を計算
    double centerX = (result.topLeft.x + result.bottomRight.x) / 2.0;

    // 旋回値の計算
    double turningPower = pid.calculatePid(centerX) * edgeSign;

    // モータのPower値をセット（前進の時0を下回らないように，後進の時0を上回らないようにセット）
    double rightPower = baseRightPower > 0.0 ? std::max(baseRightPower - turningPower, 0.0)
                                             : std::min(baseRightPower + turningPower, 0.0);
    double leftPower = baseLeftPower > 0.0 ? std::max(baseLeftPower + turningPower, 0.0)
                                           : std::min(baseLeftPower - turningPower, 0.0);
    robot.getMotorControllerInstance().setRightMotorPower(rightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(leftPower);

    // 10ms待機
    robot.getClockInstance().sleep(10000);
  }

  // モータを停止
  robot.getMotorControllerInstance().brakeWheelsMotor();
}