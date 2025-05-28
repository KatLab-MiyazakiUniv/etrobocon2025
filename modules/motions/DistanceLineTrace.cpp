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

bool DistanceLineTrace::isMetPreCondition()
{
  char buf[SMALL_BUF_SIZE];

  // targetDistance値が0以下かつtargetSpeed値が0のときwarningを出して終了する
  if(targetDistance <= 0.0 && targetSpeed == 0.0) {
    snprintf(buf, SMALL_BUF_SIZE,
             "The targetDistance value passed to DistanceLineTrace is %.2f, and the targetSpeed "
             "value passed "
             "to ColorLineTrace is 0",
             targetDistance);
    return false;
  }

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, SMALL_BUF_SIZE, "The targetSpeed value passed to DistanceLineTrace is 0");
    return false;
  }

  // targetDistance値が0以下の場合はwarningを出して終了する
  if(targetDistance <= 0.0) {
    snprintf(buf, SMALL_BUF_SIZE, "The targetDistance value passed to DistanceLineTrace is %.2f",
             targetDistance);
    return false;
  }

  return true;
}

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