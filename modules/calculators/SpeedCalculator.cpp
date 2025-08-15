/**
 *  @file SpeedCalculator.cpp
 *  @brief 目標速度に対応するPWM値を算出するクラス
 *  @author miyahara046 HaruArima08
 */
#include "SpeedCalculator.h"
#include <chrono>

using namespace std::chrono;

SpeedCalculator::SpeedCalculator(Robot& _robot, double _targetSpeed)
  : robot(_robot),
    targetSpeed(_targetSpeed),
    rightPid(RIGHT_K_P, RIGHT_K_I, RIGHT_K_D, _targetSpeed),
    leftPid(LEFT_K_P, LEFT_K_I, LEFT_K_D, _targetSpeed)
{
  double currentTime = duration_cast<duration<double>>(
        steady_clock::now().time_since_epoch()
    ).count();
  prevRightTime = currentTime;
  prevLeftTime = currentTime;
}

double SpeedCalculator::calculateRightMotorPower()
{
  spike::AllRobotStateResponse cachedState = robot.getRobotStateCacheInstance().getCachedState(); // Get cached state
  rightMotorPower
      = cachedState.rightMotorPower;  // rightMotorPowerの初期化 (from cache)
  // 走行時間を算出
  double currentRightTime = duration_cast<duration<double>>(
        steady_clock::now().time_since_epoch()
    ).count();  // 現在の時間を取得(秒単位)
  double diffRightTime = currentRightTime - prevRightTime;
  // 右タイヤの走行速度を算出
  double currentRightSpeed = cachedState.rightMotorSpeed; // From cache
  // 走行速度に相当する右タイヤのPower値を算出
  rightMotorPower += rightPid.calculatePid(currentRightSpeed, diffRightTime);
  // メンバを更新
  prevRightTime = currentRightTime;

  return rightMotorPower;
}

double SpeedCalculator::calculateLeftMotorPower()
{
  spike::AllRobotStateResponse cachedState = robot.getRobotStateCacheInstance().getCachedState(); // Get cached state
  leftMotorPower
      = cachedState.leftMotorPower;  // leftMotorPowerの初期化 (from cache)
  // 走行時間を算出
  double currentLeftTime = duration_cast<duration<double>>(
        steady_clock::now().time_since_epoch()
    ).count();  // 現在の時間を取得(秒単位)
  double diffLeftTime = currentLeftTime - prevLeftTime;
  // 左タイヤの走行速度を算出
  double currentLeftSpeed = cachedState.leftMotorSpeed; // From cache
  // 走行速度に相当する左タイヤのPower値を算出
  leftMotorPower += leftPid.calculatePid(currentLeftSpeed, diffLeftTime);
  // メンバを更新
  prevLeftTime = currentLeftTime;

  return leftMotorPower;
}
