/**
 * @file Controller.cpp
 * @brief モータ制御に用いる関数をまとめたラッパークラス
 * @author nishijima515
 */
#include "MotorController.h"

Controller::Controller()
  : rightWheel(Port::PORT_A),
    leftWheel(Port::PORT_B, Motor::EDirection::COUNTERCLOCKWISE),
    armMotor(Port::PORT_C, Motor::EDirection::COUNTERCLOCKWISE)
{
}

// power値の初期化
int Controller::powerOfRightWheel = 0.0;
int Controller::powerOfLeftWheel = 0.0;
int Controller::powerOfArm = 0.0;

// モータに設定するpower値の制限
int Controller::limitPowerValue(int inputPower)
{
  if(inputPower > MOTOR_POWER_MAX) {
    return MOTOR_POWER_MAX;
  } else if(inputPower < MOTOR_POWER_MIN) {
    return MOTOR_POWER_MIN;
  }
  return inputPower;
}

// 右モータにpower値をセット
void Controller::setRightMotorPower(int power)
{
  powerOfRightWheel = limitPowerValue(power);
  rightWheel.setPower(powerOfRightWheel);
}

// 左モータにpower値をセット
void Controller::setLeftMotorPower(int power)
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

// 右タイヤのモータに回転速度をセット
void Controller::setRightMotorSpeed(int speed)
{
  rightWheel.setSpeed(speed);
}

// 左タイヤのモータに回転速度をセット
void Controller::setLeftMotorSpeed(int speed)
{
  leftWheel.setSpeed(speed);
}

// 両タイヤのモータを停止する
void Controller::stopWheelsMotor()
{
  powerOfRightWheel = 0;
  powerOfLeftWheel = 0;
  rightWheel.stop();
  leftWheel.stop();
}

// ブレーキをかけてタイヤのモータを停止する
void Controller::brakeWheelsMotor()
{
  powerOfRightWheel = 0;
  powerOfLeftWheel = 0;
  rightWheel.brake();
  leftWheel.brake();
}

// アームのモータにpower値をセット
void Controller::setArmMotorPower(int power)
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

// アームモータを止めて角度を維持する
void Controller::holdArmMotor()
{
  armMotor.hold();
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

// アームモータのpower値を取得する
int Controller::getArmMotorPower()
{
  return armMotor.getPower();
}

// 右モータの角位置を取得する
int32_t Controller::getRightCount()
{
  return rightWheel.getCount();
}

// 右モータの角位置を取得する
int32_t Controller::getLeftCount()
{
  return leftWheel.getCount();
}

// アームモータの角位置を取得する
int32_t Controller::getArmMotorCount()
{
  return armMotor.getCount();
}

// 右タイヤモータの回転速度を取得する
int Controller::getRightMotorSpeed()
{
  return rightWheel.getSpeed();
}

// 左タイヤモータの回転速度を取得する
int Controller::getLeftMotorSpeed()
{
  return leftWheel.getSpeed();
}
