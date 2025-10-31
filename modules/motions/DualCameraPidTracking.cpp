/**
 * @file   DualCameraPidTracking.cpp
 * @brief  カメラを使った2色のPID走行の親クラス
 * @author miyahara046 HaruArima08 takuchi17
 */

#include "DualCameraPidTracking.h"
#include <thread>
#include <chrono>

DualCameraPidTracking::DualCameraPidTracking(
    Robot& _robot, double _targetSpeed, int _targetXCoordinate, const PidGain& _pidGain,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequestFirst,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequestSecond)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetXCoordinate(_targetXCoordinate),
    pidGain(_pidGain),
    detectionRequestFirst(_detectionRequestFirst),
    detectionRequestSecond(_detectionRequestSecond)
{
}

void DualCameraPidTracking::run()
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
    CameraServer::BoundingBoxDetectorResponse responseFirst;
    CameraServer::BoundingBoxDetectorResponse responseSecond;
    bool successFirst = client.executeLineDetection(detectionRequestFirst, responseFirst);
    bool successSecond = client.executeLineDetection(detectionRequestSecond, responseSecond);

    // 通信失敗、または検出できなかった場合
    if((!successFirst && !successSecond)
       || (!responseFirst.result.wasDetected && !responseSecond.result.wasDetected)) {
      continue;
    }

    // 面積を計算
    double areaFirst
        = abs((responseFirst.result.bottomRight.y - responseFirst.result.topRight.y)
              * (responseFirst.result.bottomRight.x - responseFirst.result.bottomLeft.x));
    double areaSecond
        = abs((responseSecond.result.bottomRight.y - responseSecond.result.topRight.y)
              * (responseSecond.result.bottomRight.x - responseSecond.result.bottomLeft.x));
    // 面積が大きい方の結果を採用
    // バウンディングボックスの中心X座標を計算
    double currentX
        = (areaFirst > areaSecond)
              ? (responseFirst.result.topLeft.x + responseFirst.result.bottomRight.x) / 2.0
              : (responseSecond.result.topLeft.x + responseSecond.result.bottomRight.x) / 2.0;

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

  // モータを停止
  robot.getMotorControllerInstance().stopWheelsMotor();
}