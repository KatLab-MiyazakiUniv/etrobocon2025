/**
 *  @file SpeedCalculator.cpp
 *  @brief 目標速度に対応するPWM値を算出するクラス
 *  @author miyahara046 HaruArima08
 */
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(Robot& _robot, double _targetSpeed)
  : robot(_robot),
    targetSpeed(_targetSpeed),
    rightPid(RIGHT_K_P, RIGHT_K_I, RIGHT_K_D, _targetSpeed),
    leftPid(LEFT_K_P, LEFT_K_I, LEFT_K_D, _targetSpeed)
{
  double currentTime = robot.getClockInstance().now() / 1000000.0;
  prevRightTime = currentTime;
  prevLeftTime = currentTime;
}

double SpeedCalculator::calculateRightMotorPower()
{
  rightMotorPower
      = robot.getMotorControllerInstance().getRightMotorPower();  // rightMotorPowerの初期化
  // 走行時間を算出
  double currentRightTime = robot.getClockInstance().now() / 1000000.0;  // 現在の時間を取得(秒単位)
  double diffRightTime = currentRightTime - prevRightTime;
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
  leftMotorPower
      = robot.getMotorControllerInstance().getLeftMotorPower();  // leftMotorPowerの初期化
  // 走行時間を算出
  double currentLeftTime = robot.getClockInstance().now() / 1000000.0;  // 現在の時間を取得(秒単位)
  double diffLeftTime = currentLeftTime - prevLeftTime;
  // 左タイヤの走行速度を算出
  double currentLeftSpeed = robot.getMotorControllerInstance().getLeftMotorSpeed();
  // 走行速度に相当する左タイヤのPower値を算出
  leftMotorPower += leftPid.calculatePid(currentLeftSpeed, diffLeftTime);
  // メンバを更新
  prevLeftTime = currentLeftTime;

  return leftMotorPower;
}
