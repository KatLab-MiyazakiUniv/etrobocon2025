#include "Rotation.h"
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>

Rotation::Rotation(Robot& _robot, bool _isClockwise, float _targetAngle, const PidGain& _pidGain)
  : Motion(_robot),
    robot(_robot),
    leftSign(_isClockwise ? 1 : -1),
    rightSign(_isClockwise ? -1 : 1),
    targetAngle(_targetAngle),
    pidGain(_pidGain)
{
}
Rotation::Rotation(Robot& _robot, double _speed, bool _isClockwise)
  : Motion(_robot),
    robot(_robot),
    leftSign(_isClockwise ? 1 : -1),
    rightSign(_isClockwise ? -1 : 1),
    speed(_speed)
{
}

void Rotation::run()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  MYIMU& imu = robot.getIMUInstance();

  prepare();

  if(!isMetPreCondition()) {
    std::cerr << "Rotation::run(): pre-condition not met. Abort." << std::endl;
    return;
  }

  float initialHeading = imu.getHeading();
  goalHeading = initialHeading + leftSign * targetAngle;

  Pid pid(pidGain.kp, pidGain.ki, pidGain.kd, goalHeading);

  while(isMetContinuationCondition()) {
    currentHeading = imu.getHeading();

    double correction = pid.calculatePid(currentHeading);

    motorController.setLeftMotorPower(leftSign * correction);
    motorController.setRightMotorPower(rightSign * correction);

    // 10ms待機
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  motorController.brakeWheelsMotor();
}