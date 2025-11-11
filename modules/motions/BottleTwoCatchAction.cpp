/**
 * @file   BottleTwoCatchAction.cpp
 * @brief  ボトル2つ目のキャッチ動作クラス
 * @author nishijima515
 */

#include "BottleTwoCatchAction.h"

// コンストラクタ
BottleTwoCatchAction::BottleTwoCatchAction(
    Robot& _robot, double _forwardDistance, double _idsSpeed, double _ultrasonicDistance,
    double _udclSpeed, double _angle, double _rotatePower, const PidGain& _pidGain,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : CompositeMotion(_robot),
    forwardDistance(_forwardDistance),
    idsSpeed(_idsSpeed),
    ultrasonicDistance(_ultrasonicDistance),
    udclSpeed(_udclSpeed),
    angle(_angle),
    rotatePower(_rotatePower),
    pidGain(_pidGain),
    detectionRequest(_detectionRequest)
{
}

void BottleTwoCatchAction::run()
{
  // ボトル探索開始時点の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  double initialAngle = robot.getIMUControllerInstance().getAngle();  // 動作開始時の角度

  SocketClient& client = robot.getSocketClient();
  CameraServer::BoundingBoxDetectorResponse response;
  bool success = client.executeLineDetection(detectionRequest, response);

  // PCIDSを動かす。ボトル探索。
  PictureColorDistanceStraight pcids(robot, forwardDistance, idsSpeed, PidGain(0.08, 0.02, 0.05),
                                     detectionRequest);
  pcids.run();

  UltrasonicDistanceCameraLineTrace udcl(robot, ultrasonicDistance, forwardDistance, udclSpeed, 400,
                                         pidGain, detectionRequest);

  udcl.run();
  // stop.run();

  // 動作終了時点の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  double totalAngleToTurn = robot.getIMUControllerInstance().getAngle();

  double runDistance = fabs(currentDistance - initialDistance);
  double widthMoved = runDistance * sin(totalAngleToTurn * PI / 180.0);

  std::cout << "走った距離: " << runDistance << "SCA開始時点からの傾き：" << totalAngleToTurn
            << std::endl;
  std::cout << "sin：" << sin(totalAngleToTurn) << std::endl;
  std::cout << "横方向のズレ：" << widthMoved << std::endl;

  // 回頭
  IMUMinAngleRotation minar(robot, angle, rotatePower, PidGain(0.036, 0.012, 0.03));
  minar.run();

  // 最大直進距離を設定
  double maxDistance = forwardDistance;  // forwardDistanceは1100mm程度に設定される想定
  // 補正距離を算出
  double nextDistance = maxDistance - runDistance;
  if(nextDistance < 0) {
    nextDistance = 0;
  }

  // 距離補正のための直進
  IMUDistanceStraight nextids(robot, nextDistance, idsSpeed, PidGain(0.08, 0.02, 0.05));
  nextids.run();
}