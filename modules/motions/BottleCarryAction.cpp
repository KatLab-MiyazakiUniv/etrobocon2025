/**
 * @file   BottleCarryAction.cpp
 * @brief  ボトルキャリー動作クラス
 * @author nishijima515
 */

#include "BottleCarryAction.h"
#include <thread>
#include <iostream>

using namespace std;
// コンストラクタ
// offsetDistance: 距離補正値[mm]
// idsSpeed: IDSの走行速度[mm/s]
BottleCarryAction::BottleCarryAction(
    Robot& _robot, double _offsetDistance, double _idsSpeed,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : CompositeMotion(_robot),
    offsetDistance(_offsetDistance),
    idsSpeed(_idsSpeed),
    detectionRequest(_detectionRequest)
{
}

// カメラ距離計算、ランディングのためのIDSを担当する複合動作クラス
void BottleCarryAction::run()
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

// // IDSを動かす。青丸探索。
// while(1) {
//   bool success = client.executeLineDetection(detectionRequest, response);
//   if(success && response.result.wasDetected) {
//     // ボトルが検出された場合の処理
//     break;
//   }
//   //     std::cout << "SmartCarryAction: IDS start" << std::endl;
//   //
//   現状だと、バウンディングボックスを検出できなかったら、無限に直進するため、指定した距離に到達したらwhileループを抜ける処理を追加する
//   ids.run();
// }

// Snapshot ss(robot, "testtest");
// for(int i = 0; i < 5; i++) {
//   ss.run();
// }

// // 青丸に直進するための補正角度計算
// while(1) {
//   GetCorrectionAngle correction(robot);
//   GetCorrectionAngleResult correctionResult;
//   correctionResult = correction.GetCorrectAngle(targetXCoordinate, detectionRequest);
//   std::cout << "補正回頭角度：" << correctionResult.correctionAngle << std::endl;
//   IMUAngleRotation rotate(robot, correctionResult.correctionAngle, 60.0,
//                           correctionResult.isClockwise, PidGain(0.036, 0.012, 0.03), false);
//   if(correctionResult.correctionAngle <= 3.0) {
//     rotate.run();
//     break;
//   }
//   rotate.run();
// }

// DCLで補正回頭2回目
// DistanceCameraLineTrace correct2(robot, dclDistance, Speed, targetXCoordinate,
//                                  PidGain(0.002, 0.0005, 0.001), detectionRequest);
// correct2.run();

// // 青丸が画角に写るギリギリまでPCIDSで直進
// CameraServer::BoundingBoxDetectorRequest pcidsdetectionRequest;
// pcidsdetectionRequest.command = detectionRequest.command;
// pcidsdetectionRequest.lowerHSV = detectionRequest.lowerHSV;
// pcidsdetectionRequest.upperHSV = detectionRequest.upperHSV;
// pcidsdetectionRequest.roi = pcidsRoi;
// PictureColorDistanceStraight pcids(robot, cameraFirstDistance, idsSpeed, PidGain(0.08, 0.02,
// 0.05),
//                                   pcidsdetectionRequest);
// pcids.run();

// 　IMU設定
// IMUSetting start(robot, true);
// IMUSetting stop(robot, false);

// start.run();
// IMUDistanceStraight ids(robot, forwardDistance, idsSpeed, PidGain(0.08, 0.02, 0.05));

// // 動作安定のためのスリープ
// this_thread::sleep_for(chrono::milliseconds(10));