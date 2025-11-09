// 距離計測の開始
// if 画角に指定した色が入るまで、5cmごと前進を続ける

/**
 * @file   SmartCarryAction.cpp
 * @brief  スマートキャリー動作クラス
 * @author nishijima515
 */

#include "SmartCarryAction.h"

// コンストラクタ
SmartCarryAction::SmartCarryAction(
    Robot& _robot, double _forwardDistance, double _idsSpeed, double _ultrasonicDistance,
    double _udclDistance, const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : CompositeMotion(_robot),
    forwardDistance(_forwardDistance),
    idsSpeed(_idsSpeed),
    ultrasonicDistance(_ultrasonicDistance),
    udclDistance(_udclDistance),
    detectionRequest(_detectionRequest)
{
}

void SmartCarryAction::run()
{
  // UDCLを動かす。青ボトル目標。（超音波距離）UDCL,70.0,5000,470,400,0.002,0.0005,0.001,85,50,50,105,255,255,0,0,600,600,800,600
  //   UltrasonicDistanceCameraLineTrace udcl(robot, 70.0, ultrasonicDistance, 470, 400,
  //                                          PidGain(0.002, 0.0005, 0.001), std::move(detector));
  // ボトル探索開始時点の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  //   std::cout << "SmartCarryAction: UDCL start" << std::endl;
  //   std::cout << "UDCL開始時点の走行距離: " << initialDistance << std::endl;

  // // 　IMU設定
  // IMUSetting start(robot, true);
  // IMUSetting stop(robot, false);

  double initialAngle = robot.getIMUControllerInstance().getAngle();  // 動作開始時の角度

  // start.run();
  // IDSを動かす。ボトル探索。
  IMUDistanceStraight ids(robot, forwardDistance, idsSpeed, PidGain(0.08, 0.02, 0.05));

  SocketClient& client = robot.getSocketClient();
  CameraServer::BoundingBoxDetectorResponse response;

  while(1) {
    bool success = client.executeLineDetection(detectionRequest, response);
    if(success && response.result.wasDetected) {
      // ボトルが検出された場合の処理
      break;
    }
    //     std::cout << "SmartCarryAction: IDS start" << std::endl;
    ids.run();
  }

  UltrasonicDistanceCameraLineTrace udcl(robot, ultrasonicDistance, udclDistance, 470, 400,
                                         PidGain(0.002, 0.0005, 0.001), detectionRequest);
  
  udcl.run();
  // stop.run();

  // 動作終了時点の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  double totalAngleToTurn = robot.getIMUControllerInstance().getAngle();

  double runDistance = fabs(currentDistance - initialDistance);
  double widthMoved = runDistance * sin(totalAngleToTurn * PI / 180.0);

  std::cout << "走った距離: " << runDistance << "SCA開始時点からの傾き：" << totalAngleToTurn <<std::endl;
  std::cout << "sin：" << sin(totalAngleToTurn) << std::endl;
  std::cout << "横方向のズレ：" << widthMoved << std::endl;

  IMUAngleRotation imur(robot, 340, 60.0, false, PidGain(0.036, 0.012, 0.03),
                         true);
  imur.run();

  double nextDistance = 1100.0 - runDistance;
  if(nextDistance < 0){
    nextDistance = 0;
  }



  IMUDistanceStraight nextids(robot, nextDistance, idsSpeed, PidGain(0.08, 0.02, 0.05));
  nextids.run();

}