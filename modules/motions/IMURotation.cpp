/**
 * @file   IMURotation.cpp
 * @brief  IMU角度指定回頭動作
 * @author Hara1274
 */

#include "IMURotation.h"

IMURotation::IMURotation(Robot& _robot, int _targetAngle, double _power, bool _isClockwise)
  : Rotation(_robot, _power, _isClockwise),
    targetAngle(_targetAngle),
    power(_power)
{
}

void IMURotation::prepare()
{
  // IMU角度計算開始
  robot.getIMUControllerInstance().startAngleCalculation();
}

bool IMURotation::isMetPreCondition()
{
  return true;
}

bool IMURotation::isMetContinuationCondition()
{
  // 現在角度を取得
  float currentAngle = abs(robot.getIMUControllerInstance().getAngle());
  
  // 目標角度に到達したら停止
  if(currentAngle >= targetAngle) {
    return false;
  }
  return true;
}

void IMURotation::setMotorControl()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  // setPowerを使用
  motorController.setLeftMotorPower(power * leftSign);
  motorController.setRightMotorPower(power * rightSign);
}