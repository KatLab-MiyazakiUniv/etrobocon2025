/**
 * @file   DistanceLineTrace.cpp
 * @brief  指定距離ライントレース動作
 * @author miyahara046 nishijima515
 */

#include "DistanceLineTrace.h"

DistanceLineTrace::DistanceLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                                     int _targetBrightness, const PidGain& _pidGain)
  : LineTrace(_robot, _targetSpeed, _targetBrightness, _pidGain), targetDistance(_targetDistance)
{
}

// 距離ライントレースの事前条件
bool DistanceLineTrace::isMetPreCondition()
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

// 距離ライントレースの事前処理
void DistanceLineTrace::prepare()
{
  // 初期値を代入 (RobotStateCacheから)
  spike::AllRobotStateResponse cachedState = robot.getRobotStateCacheInstance().getCachedState();
  initDistance = Mileage::calculateMileage(cachedState.rightMotorCount,
                                           cachedState.leftMotorCount);
}

// 距離ライントレースの継続条件
bool DistanceLineTrace::isMetContinuationCondition()
{
  // 走行距離が目標距離に到達
  spike::AllRobotStateResponse cachedState = robot.getRobotStateCacheInstance().getCachedState();
  if(fabs(Mileage::calculateMileage(cachedState.rightMotorCount,
                                    cachedState.leftMotorCount)
          - initDistance)
     >= targetDistance)
    return false;

  return true;
}