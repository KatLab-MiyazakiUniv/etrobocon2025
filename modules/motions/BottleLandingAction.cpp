/**
 * @file   BottleLandingAction.cpp
 * @brief  ボトルランディング動作クラス
 * @author nishijima515
 */

#include "BottleLandingAction.h"
#include <thread>
#include <iostream>

using namespace std;
// コンストラクタ
// offsetDistance: 距離補正値[mm]
// idsSpeed: IDSの走行速度[mm/s]
BottleLandingAction::BottleLandingAction(
    Robot& _robot, double _offsetDistance, double _idsSpeed,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : CompositeMotion(_robot),
    offsetDistance(_offsetDistance),
    idsSpeed(_idsSpeed),
    detectionRequest(_detectionRequest)
{
}

// カメラ距離計算、ランディングのためのIDSを担当する複合動作クラス
void BottleLandingAction::run()
{
  // 動作開始時点の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  SocketClient& client = robot.getSocketClient();
  CameraServer::BoundingBoxDetectorResponse response;
  bool success = client.executeLineDetection(detectionRequest, response);

  // 青丸までの距離をカメラで計測
  CameraDistanceCalculator calculator(robot);
  double cameraDistance = calculator.calculateDistance(response, offsetDistance);
  std::cout << "CameraDistance: " << cameraDistance << std::endl;

  IMUDistanceStraight idss(robot, cameraDistance, idsSpeed, PidGain(0.08, 0.02, 0.05));
  idss.run();

  // 動作終了時点の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  double runDistance = fabs(currentDistance - initialDistance);

  std::cout << "走った距離: " << runDistance << std::endl;
}