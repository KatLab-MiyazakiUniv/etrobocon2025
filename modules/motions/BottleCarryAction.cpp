/**
 * @file   BottleCarryAction.cpp
 * @brief  ボトルキャリー動作クラス
 * @author nishijima515
 */

#include "BottleCarryAction.h"

// コンストラクタ
BottleCarryAction::BottleCarryAction(
    Robot& _robot, double _forwardDistance, double _maxDistance, double _idsSpeed,
    int _targetXCoordinate, const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : CompositeMotion(_robot),
    forwardDistance(_forwardDistance),
    maxDistance(_maxDistance),
    idsSpeed(_idsSpeed),
    targetXCoordinate(_targetXCoordinate),
    detectionRequest(_detectionRequest)
{
}

void BottleCarryAction::run()
{
  // 探索開始時点の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // 　IMU設定
  IMUSetting start(robot, true);
  IMUSetting stop(robot, false);

  start.run();
  IMUDistanceStraight ids(robot, forwardDistance, idsSpeed, PidGain(0.08, 0.02, 0.05));

  SocketClient& client = robot.getSocketClient();
  CameraServer::BoundingBoxDetectorResponse response;

  // IDSを動かす。青丸探索。
  while(1) {
    bool success = client.executeLineDetection(detectionRequest, response);
    if(success && response.result.wasDetected) {
      // ボトルが検出された場合の処理
      break;
    }
    //     std::cout << "SmartCarryAction: IDS start" << std::endl;
    // 現状だと、バウンディングボックスを検出できなかったら、無限に直進するため、指定した距離に到達したらwhileループを抜ける処理を追加する
    ids.run();
  }

  // 青丸に直進するための補正角度計算
  GetCorrectionAngle correction(robot);
  GetCorrectionAngleResult correctionResult;
  correctionResult = correction.GetCorrectAngle(targetXCoordinate, detectionRequest);
  IMUAngleRotation rotate(robot, correctionResult.correctionAngle, 70.0,
                          correctionResult.isClockwise, PidGain(0.036, 0.012, 0.03), false);
  rotate.run();
  // 青丸までの距離をカメラで計測
  CameraDistanceCalculator calculator(robot);
  double cameraDistance = calculator.calculateDistance(response);
  std::cout << "Camera Distance: " << cameraDistance << std::endl;
  // 色距離指定IMU直進
  //   IMUColorDistanceStraight(Robot& _robot, COLOR _targetColor, double _targetDistance,
  //                            double _targetSpeed, const PidGain& _anglePidGain);
  IMUColorDistanceStraight icds(robot, COLOR::BLUE, cameraDistance, idsSpeed,
                                PidGain(0.08, 0.02, 0.05));
  icds.run();

  // 動作終了時点の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  double runDistance = fabs(currentDistance - initialDistance);

  std::cout << "走った距離: " << runDistance << std::endl;
  stop.run();
}