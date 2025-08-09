/**
 * @file   Rotation.cpp
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#include "Rotation.h"
#include <iostream>

Rotation::Rotation(Robot& _robot, double _speed, bool _isClockwise)
  : Motion(_robot),
    speed(_speed),
    isClockwise(_isClockwise),
    leftSign(_isClockwise ? 1 : -1),
    rightSign(_isClockwise ? -1 : 1)
{
}

void Rotation::run()
{
  MotorController& motorController = robot.getMotorControllerInstance();

  prepare();
  if(!isMetPreCondition()) return;

  // 回転速度（mm/秒）で指定しモーターを制御
  motorController.setLeftMotorSpeed(speed * leftSign);
  motorController.setRightMotorSpeed(speed * rightSign);

  while(isMetContinuationCondition()) {
    // 回頭継続条件が満たされるまで待機（モーターが走行中）
  }
  // モーターを停止
  motorController.stopWheelsMotor();

  robot.getIMUControllerInstance().stopAngleCalculation();
  // 最終角度を取得してIMU測定停止
  float finalAngle = robot.getIMUControllerInstance().getAngle();
  std::cout << "回頭後の角度: " << finalAngle << " deg" << std::endl;
}
