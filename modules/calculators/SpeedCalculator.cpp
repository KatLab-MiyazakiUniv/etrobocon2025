/**
 *  @file SpeedCalculator.cpp
 *  @brief 目標速度に対応するPWM値を算出するクラス
 *  @author CHIHAYATAKU
 */
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(Robot& _robot, double _targetSpeed)
  : robot(_robot),
    targetSpeed(_targetSpeed),
    rightPid(K_P, K_I, K_D, _targetSpeed),
    leftPid(K_P, K_I, K_D, _targetSpeed)
{
  int currentTime = robot.getClockInstance().now();
  prevRightTime = currentTime;
  prevLeftTime = currentTime;
}

double SpeedCalculator::calculateRightMotorPowor()
{
  // 走行時間を算出
  int currentRightTime = robot.getClockInstance().now();
  double diffRightTime = (double)(currentRightTime - prevRightTime);
  // 右タイヤの走行速度を算出
  double currentRightAngleSpeed = robot.getMotorControllerInstance().getRightMotorSpeed();
  double currentRightSpeed = currentRightAngleSpeed / 180 * RADIUS * 3.14;
  // 走行速度に相当する右タイヤのPWM値を算出
  rightMotorPower += rightPid.calculatePid(currentRightSpeed, diffRightTime);
  // メンバを更新
  prevRightTime = currentRightTime;

  return rightMotorPower;
}

double SpeedCalculator::calculateLeftMotorPowor()
{
  // 走行時間を算出
  int currentLeftTime = robot.getClockInstance().now();
  double diffLeftTime = (double)(currentLeftTime - prevLeftTime);
  // 右タイヤの走行速度を算出
  double currentLeftAngleSpeed = robot.getMotorControllerInstance().getLeftMotorSpeed();
  double currentLeftSpeed = currentLeftAngleSpeed / 180 * RADIUS * 3.14;
  // 走行速度に相当する右タイヤのPWM値を算出
  leftMotorPower += leftPid.calculatePid(currentLeftSpeed, diffLeftTime);
  // メンバを更新
  prevLeftTime = currentLeftTime;

  return leftMotorPower;
}
