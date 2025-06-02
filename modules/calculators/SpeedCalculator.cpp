/**
 *  @file SpeedCalculator.cpp
 *  @brief 目標速度に対応するPWM値を算出するクラス
 *  @author miyahara046, nishijima515
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

double SpeedCalculator::calculateRightMotorPower()
{
  // 走行時間を算出
  int currentRightTime = robot.getClockInstance().now();
  double diffRightTime = (double)(currentRightTime - prevRightTime);
  // 右タイヤの走行速度を算出
  double currentRightSpeed = robot.getMotorControllerInstance().getRightMotorSpeed();
  // 走行速度に相当する右タイヤのPower値を算出
  rightMotorPower += rightPid.calculatePid(currentRightSpeed, diffRightTime);
  // メンバを更新
  prevRightTime = currentRightTime;

  return rightMotorPower;
}

double SpeedCalculator::calculateLeftMotorPower()
{
  // 走行時間を算出
  int currentLeftTime = robot.getClockInstance().now();
  double diffLeftTime = (double)(currentLeftTime - prevLeftTime);
  // 左タイヤの走行速度を算出
  double currentLeftSpeed = robot.getMotorControllerInstance().getLeftMotorSpeed();
  // 走行速度に相当する左タイヤのPower値を算出
  leftMotorPower += leftPid.calculatePid(currentLeftSpeed, diffLeftTime);
  // メンバを更新
  prevLeftTime = currentLeftTime;

  return leftMotorPower;
}
