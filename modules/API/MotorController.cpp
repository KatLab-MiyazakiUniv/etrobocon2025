/**
 * @file MotorController.cpp
 * @brief モータ制御に用いる関数をまとめたラッパークラス
 * @author nishijima515
 */
#include "MotorController.h"
using namespace spikeapi;

MotorController::MotorController()
  : rightWheel(EPort::PORT_A),
    leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE),
    armMotor(EPort::PORT_C, Motor::EDirection::COUNTERCLOCKWISE)
{
}

// モータに設定するpower値の制限
int MotorController::limitPowerValue(int inputPower)
{
  if(inputPower > MOTOR_POWER_MAX) {
    return MOTOR_POWER_MAX;
  } else if(inputPower < MOTOR_POWER_MIN) {
    return MOTOR_POWER_MIN;
  }
  return inputPower;
}

// 右モータにpower値をセット
void MotorController::setRightMotorPower(int power)
{
  rightWheel.setPower(limitPowerValue(power));
}

// 左モータにpower値をセット
void MotorController::setLeftMotorPower(int power)
{
  leftWheel.setPower(limitPowerValue(power));
}

// 右モータのpower値をリセット
void MotorController::resetRightMotorPower()
{
  rightWheel.setPower(0);
}

// 左モータのpower値をリセット
void MotorController::resetLeftMotorPower()
{
  leftWheel.setPower(0);
}

// 右左両モータの状態をリセット
void MotorController::resetWheelsMotorPower()
{
  rightWheel.setPower(0);
  leftWheel.setPower(0);
}

// 右タイヤのモータに,線速度を回転速度に変換しセットする
void MotorController::setRightMotorSpeed(double speed)
{
  int rightAngleSpeed = static_cast<int>(speed / WHEEL_RADIUS * (RAD_TO_DEG));
  rightWheel.setSpeed(rightAngleSpeed);
}

// 左タイヤのモータに,線速度を回転速度に変換しセットする
void MotorController::setLeftMotorSpeed(double speed)
{
  int leftAngleSpeed = static_cast<int>(speed / WHEEL_RADIUS * (RAD_TO_DEG));
  leftWheel.setSpeed(leftAngleSpeed);
}

// 両タイヤのモータを停止する
void MotorController::stopWheelsMotor()
{
  rightWheel.stop();
  leftWheel.stop();
}

// ブレーキをかけてタイヤのモータを停止する
void MotorController::brakeWheelsMotor()
{
  rightWheel.brake();
  leftWheel.brake();
}

// アームのモータにpower値をセット
void MotorController::setArmMotorPower(int power)
{
  armMotor.setPower(limitPowerValue(power));
}

// アームのモータのpower値をリセット
void MotorController::resetArmMotorPower()
{
  armMotor.setPower(0);
}

// アームのモータを停止する
void MotorController::stopArmMotor()
{
  armMotor.stop();
}

// アームモータを止めて角度を維持する
void MotorController::holdArmMotor()
{
  armMotor.hold();
}

// 右タイヤのpower値を取得する
int MotorController::getRightMotorPower()
{
  return rightWheel.getPower();
}

// 左タイヤのpower値を取得する
int MotorController::getLeftMotorPower()
{
  return leftWheel.getPower();
}

// アームモータのpower値を取得する
int MotorController::getArmMotorPower()
{
  return armMotor.getPower();
}

// 右モータの角位置を取得する
int32_t MotorController::getRightMotorCount()
{
  return rightWheel.getCount();
}

// 左モータの角位置を取得する
int32_t MotorController::getLeftMotorCount()
{
  return leftWheel.getCount();
}

// アームモータの角位置を取得する
int32_t MotorController::getArmMotorCount()
{
  return armMotor.getCount();
}

// 右タイヤモータの線速度を取得する
double MotorController::getRightMotorSpeed()
{
  double rightSpeed = rightWheel.getSpeed() * DEG_TO_RAD * WHEEL_RADIUS;
  return rightSpeed;
}

// 左タイヤモータの線速度を取得する
double MotorController::getLeftMotorSpeed()
{
  double leftSpeed = leftWheel.getSpeed() * DEG_TO_RAD * WHEEL_RADIUS;
  return leftSpeed;
}
