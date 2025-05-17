/**
 * @file Controller.cpp
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author takahashitom CHIHAYATAKU
 */
#include "Controller.h"

Controller::Controller()
  : rightWheel(Port::PORT_A),
    leftWheel(Port::PORT_B, Motor::EDirection::COUNTERCLOCKWISE),
    armMotor(Port::PORT_C)
{
}

// power値の初期化
int Controller::powerOfRightWheel = 0.0;
int Controller::powerOfLeftWheel = 0.0;
int Controller::powerOfArm = 0.0;

// モータに設定するPWM値の制限
int Controller::limitPowerValue(const int inputPower)
{
  if(inputPower > MOTOR_POWER_MAX) {
    return MOTOR_POWER_MAX;
  } else if(inputPower < MOTOR_POWER_MIN) {
    return MOTOR_POWER_MIN;
  }
  return inputPower;
}

// 右モータにpower値をセット
void Controller::setRightMotorPower(const int power)
{
  powerOfRightWheel = limitPowerValue(power);
  rightWheel.setPower(powerOfRightWheel);
}

// 左モータにpower値をセット
void Controller::setLeftMotorPower(const int power)
{
  powerOfLeftWheel = limitPowerValue(power);
  leftWheel.setPower(powerOfLeftWheel);
}

// 右モータのpower値をリセット
void Controller::resetRightMotorPower()
{
  powerOfRightWheel = 0;
  rightWheel.setPower(powerOfRightWheel);
}

// 左モータのpower値をリセット
void Controller::resetLeftMotorPower()
{
  powerOfLeftWheel = 0;
  leftWheel.setPower(powerOfLeftWheel);
}

// 右左両モータの状態をリセット
void Controller::resetWheelsMotorPower()
{
  powerOfRightWheel = 0;
  powerOfLeftWheel = 0;
  rightWheel.setPower(powerOfRightWheel);
  leftWheel.setPower(powerOfLeftWheel);
}

// タイヤのモータを停止する
void Controller::stopWheelsMotor()
{
  powerOfRightWheel = 0;
  powerOfLeftWheel = 0;
  rightWheel.stop();
  leftWheel.stop();
}

// アームのモータにpower値をセット
void Controller::setArmMotorPower(const int power)
{
  powerOfArm = limitPowerValue(power);
  armMotor.setPower(powerOfArm);
}

// アームのモータのpower値をリセット
void Controller::resetArmMotorPower()
{
  powerOfArm = 0;
  armMotor.setPower(powerOfArm);
}

// アームのモータを停止する
void Controller::stopArmMotor()
{
  powerOfArm = 0;
  armMotor.stop();
}

// 右タイヤのpower値を取得する
int Controller::getRightMotorPower()
{
  return rightWheel.getPower();
}

// 左タイヤのpower値を取得する
int Controller::getLeftMotorPower()
{
  return leftWheel.getPower();
}