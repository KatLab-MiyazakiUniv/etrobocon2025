/**
 * @file   SmartCarryAction.cpp
 * @brief  スマートキャリー動作クラス
 * @author nishijima515
 */

#include "SmartCarryAction.h"

// コンストラクタ
SmartCarryAction::SmartCarryAction(Robot& _robot, double _ultrasonicDistance, int _imudeg,
                                   double _imupower, bool _isClockwise, double _dsSpeed)
  : CompositeMotion(_robot),
    ultrasonicDistance(_ultrasonicDistance),
    imuDeg(_imudeg),
    imuPower(_imupower),
    isClockwise(_isClockwise),
    dsSpeed(_dsSpeed)
{
}

void SmartCarryAction::run()
{
  // UDCLを動かす。青ボトル目標。（超音波距離）UDCL,70.0,5000,470,400,0.002,0.0005,0.001,85,50,50,105,255,255,0,0,600,600,800,600
  UltrasonicDistanceCameraLineTrace udcl(robot, 70.0, ultrasonicDistance, 470, 400,
                                         PidGain(0.002, 0.0005, 0.001), std::move(detector));
  // UDCL開始時点の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // UDCL実行
  udcl.run();

  // UDCL終了時点の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  // DSで進むべき距離を計算する。(超音波距離 - 走行距離)
  double runDistance = fabs(currentDistance - initialDistance);
  double distanceToMove = ultrasonicDistance - runDistance;

  // IMUR回頭動作（20度）調整可能にしておく IMUR,45,70.0,anticlockwise,0.036,0.012,0.03
  IMURotation imur(robot, imuDeg, imuPower, isClockwise, PidGain(0.036, 0.012, 0.03));
  imur.run();

  // DS (スピード)
  DistanceStraight ds(robot, distanceToMove, dsSpeed);
  ds.run();
}