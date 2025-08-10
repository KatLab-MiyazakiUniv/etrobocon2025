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
  float currentAngle = robot.getIMUControllerInstance().getAngle();
  
  // IMUは時計回りをマイナス、反時計回りをプラスで出力
  if(isClockwise) {
    // 時計回り: currentAngle <= -targetAngle で停止
    return currentAngle > -targetAngle;
  } else {
    // 反時計回り: currentAngle >= targetAngle で停止
    return currentAngle < targetAngle;
  }
}

void IMURotation::setMotorControl()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  // setPowerを使用
  motorController.setLeftMotorPower(power * leftSign);
  motorController.setRightMotorPower(power * rightSign);
}

void IMURotation::updateMotorControl()
{
  // PID制御等を実装する場合はここに追加
}