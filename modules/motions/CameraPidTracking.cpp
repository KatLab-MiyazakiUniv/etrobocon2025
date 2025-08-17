/**
 * @file   CameraPidTracking.cpp
 * @brief  カメラを使ったPID走行の親クラス
 * @author miyahara046 HaruArima08
 */

#include "CameraPidTracking.h"
#include <thread>
#include <chrono>

CameraPidTracking::CameraPidTracking(
    Robot& _robot, double _targetSpeed, int _targetXCoordinate, const PidGain& _pidGain,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetXCoordinate(_targetXCoordinate),
    pidGain(_pidGain),
    detectionRequest(_detectionRequest)
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

  // 左右で符号を変える
  int edgeSign = robot.getIsLeftEdge() ? -1 : 1;

  SpeedCalculator speedCalculator(robot, targetSpeed);

  // Get SocketClient from Robot
  SocketClient& client = robot.getSocketClient();

  // 継続条件を満たしている間ループ
  while(isMetContinuationCondition()) {
    // 初期Speed値を計算
    double baseRightPower = speedCalculator.calculateRightMotorPower();
    double baseLeftPower = speedCalculator.calculateLeftMotorPower();

    // Execute line detection on server
    CameraServer::BoundingBoxDetectorResponse response;
    bool success = client.executeLineDetection(detectionRequest, response);

    // If detection failed, skip this iteration
    if(!success || !response.result.wasDetected) {
      continue;
    }

    // バウンディングボックスの中心X座標を計算
    double currentX = (response.result.topLeft.x + response.result.bottomRight.x) / 2.0;

    // 旋回値の計算
    double turningPower = pid.calculatePid(currentX) * edgeSign;

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