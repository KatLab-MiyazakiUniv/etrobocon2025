/**
 * @file MotorController.cpp
 * @brief モータ制御に用いる関数をまとめたラッパークラス
 * @author nishijima515
 */
#include "MotorController.h"

MotorController::MotorController()
  : rightWheel(Port::PORT_A),
    leftWheel(Port::PORT_B, Motor::EDirection::COUNTERCLOCKWISE),
    armMotor(Port::PORT_C, Motor::EDirection::COUNTERCLOCKWISE)
{
}

// power値の初期化
int MotorController::powerOfRightWheel = 0;
int MotorController::powerOfLeftWheel = 0;
int MotorController::powerOfArm = 0;

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
  powerOfRightWheel = limitPowerValue(power);
  rightWheel.setPower(powerOfRightWheel);
}

// 左モータにpower値をセット
void MotorController::setLeftMotorPower(int power)
{
  powerOfLeftWheel = limitPowerValue(power);
  leftWheel.setPower(powerOfLeftWheel);
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

// 右タイヤのモータに回転速度をセット
void MotorController::setRightMotorSpeed(int speed)
{
  rightWheel.setSpeed(speed);
}

// 左タイヤのモータに回転速度をセット
void MotorController::setLeftMotorSpeed(int speed)
{
  leftWheel.setSpeed(speed);
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
  powerOfArm = limitPowerValue(power);
  armMotor.setPower(powerOfArm);
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

// 右タイヤモータの回転速度を取得する
int32_t MotorController::getRightMotorSpeed()
{
  return rightWheel.getSpeed();
}

// 左タイヤモータの回転速度を取得する
int32_t MotorController::getLeftMotorSpeed()
{
  return leftWheel.getSpeed();
}
