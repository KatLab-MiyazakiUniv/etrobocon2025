#include "Rotation.h"
#include <iostream>
#include <thread>
#include <chrono>

Rotation::Rotation(Robot& _robot, bool _isClockwise, int _targetAngle, const PidGain& _pidGain)
  : Motion(_robot),
    leftSign(_isClockwise ? 1 : -1),
    rightSign(_isClockwise ? -1 : 1),
    targetAngle(_targetAngle),
    pidGain(_pidGain)
{
}

void Rotation::run()
{
  // 事前条件を判定
  if (!isMetPreCondition()) {
    return;
  }

  prepare();

  // 目標角速度 90deg/s をラジアンに変換
  const double targetAngularVelocityRad = 720.0 * DEG_TO_RAD;

  // トレッド幅の半分 (mm)
  const double halfTread = TREAD / 2.0;

  // 車体中心の線速度 (mm/s)
  const double targetLinearVelocity = targetAngularVelocityRad * halfTread;

  // SpeedCalculator は正の速度のみ受け取る想定なので絶対値で渡す
  spikeapi::MotorController& motorController = robot.getMotorControllerInstance();
  motorController.setRightMotorSpeed(targetLinearVelocity * rightSign);
  motorController.setLeftMotorSpeed(targetLinearVelocity * leftSign);

  while (isMetContinuationCondition()) {
  }

  robot.getMotorControllerInstance().brakeWheelsMotor();
}


