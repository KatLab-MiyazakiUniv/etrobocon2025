/**
 * @file   DistanceLineTrace.cpp
 * @brief  指定距離ライントレース動作
 * @author miyahara046
 */

#include "DistanceLineTrace.h"
using namespace std;

DistanceLineTrace::DistanceLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                                     int _targetBrightness, const PidGain& _pidGain,
                                     bool& _isLeftEdge)
  : LineTrace(_robot, _targetSpeed, _targetBrightness, _pidGain, _isLeftEdge),
    targetDistance(_targetDistance)
{
}

// 距離ライントレースの事前条件
bool DistanceLineTrace::isMetPreCondition()
{
  // targetDistance値が0以下かつtargetSpeed値が0のとき終了する
  if(targetDistance <= 0.0 && targetSpeed == 0.0) {
    return false;
  }

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
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());

  // 呼び出し時の走行距離
  initLeftMileage
      = Mileage::calculateWheelMileage(robot.getMotorControllerInstance().getLeftMotorCount());
  initRightMileage
      = Mileage::calculateWheelMileage(robot.getMotorControllerInstance().getRightMotorCount());
}

// 距離ライントレースの継続条件
bool DistanceLineTrace::isMetContinuationCondition()
{
  // 走行距離が目標距離に到達
  if(fabs(Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                    robot.getMotorControllerInstance().getLeftMotorCount())
          - initDistance)
     >= targetDistance)
    return false;

  return true;
}