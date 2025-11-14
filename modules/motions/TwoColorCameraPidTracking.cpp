/**
 * @file   TwoColorCameraPidTracking.cpp
 * @brief  カメラを使った2色指定PID走行の親クラス
 * @author miyahara046 HaruArima08 takuchi17
 */

#include "TwoColorCameraPidTracking.h"
#include <thread>
#include <chrono>

TwoColorCameraPidTracking::TwoColorCameraPidTracking(
    Robot& _robot, double _targetSpeed, int _targetXCoordinate, const PidGain& _pidGain,
    const CameraServer::TwoColorBoundingBoxDetectorRequest& _detectionRequest,
    bool _isStopMotorPower)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetXCoordinate(_targetXCoordinate),
    pidGain(_pidGain),
    detectionRequest(_detectionRequest),
    isStopMotorPower(_isStopMotorPower)
{
}

void TwoColorCameraPidTracking::run()
{
  Pid pid(pidGain.kp, pidGain.ki, pidGain.kd, targetXCoordinate);
  // 事前条件を判定する
  if(!isMetPreCondition()) {
    return;
  }

  // 事前準備
  prepare();

  SpeedCalculator speedCalculator(robot, targetSpeed);

  SocketClient& client = robot.getSocketClient();

  // 継続条件を満たしている間ループ
  while(isMetContinuationCondition()) {
    // 初期Speed値を計算
    double baseRightPower = speedCalculator.calculateRightMotorPower();
    double baseLeftPower = speedCalculator.calculateLeftMotorPower();

    // ライン検出をサーバーに依頼
    CameraServer::BoundingBoxDetectorResponse response;
    bool success = client.executeTwoColorLineDetection(detectionRequest, response);

    // 通信失敗、または検出できなかった場合
    if(!success || !response.result.wasDetected) {
      continue;
    }

    // バウンディングボックスの中心X座標を計算
    double currentX = (response.result.topLeft.x + response.result.bottomRight.x) / 2.0;

    // 旋回値の計算
    double turningPower = pid.calculatePid(currentX) * -1;

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

  // 後続動作（例: CRA）で停止する場合はモータを維持するため、必要なときだけ停止
  if(isStopMotorPower) {
    robot.getMotorControllerInstance().stopWheelsMotor();
  }
}