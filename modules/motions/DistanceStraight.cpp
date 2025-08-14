/**
 * @file   DistanceStraight.cpp
 * @brief  目標距離まで直進するクラス
 * @author HaruArima08
 */

#include "DistanceStraight.h"
#include <iostream>

DistanceStraight::DistanceStraight(Robot& _robot, double _targetDistance, double _targetSpeed)
  : Straight(_robot, _targetSpeed), targetDistance(_targetDistance)
{
}

bool DistanceStraight::isMetPreCondition()
{
  // targetSpeed値が0の場合は終了する
  if(targetSpeed == 0.0) {
    return false;
  }

  // targetDistance値が0以下の場合は終了する
  if(targetDistance <= 0.0) {
    return false;
  }

  return true;
}

void DistanceStraight::prepare()
{
  // 呼び出し時の走行距離を取得する (RobotStateCacheから)
  spike::AllRobotStateResponse cachedState = robot.getRobotStateCacheInstance().getCachedState();
  double initialRightMotorCount = cachedState.rightMotorCount;
  double initialLeftMotorCount = cachedState.leftMotorCount;
  initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);
}

bool DistanceStraight::isMetContinuationCondition()
{
  // 現在の走行距離を取得する (RobotStateCacheから)
  spike::AllRobotStateResponse cachedState = robot.getRobotStateCacheInstance().getCachedState();
  double currentRightMotorCount = cachedState.rightMotorCount;
  double currentLeftMotorCount = cachedState.leftMotorCount;
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);
  std::cout << currentDistance << std::endl;

  // 現在の走行距離が目標走行距離に達した場合falseを返す
  // ここでは、走行体全体の距離 (左右の距離の平均) が達したかどうかで判断する
  if((fabs(currentDistance - initialDistance) >= targetDistance)) {
    return false;
  }

  // 現在の走行距離が目標走行距離に達していなければ走行を続ける
  return true;
}
