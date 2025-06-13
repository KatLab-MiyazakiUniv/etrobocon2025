/**
 * @file   DistanceLineTrace.cpp
 * @brief  指定距離ライントレース動作
 * @author miyahara046
 */

#include "DistanceCameraLineTrace.h"

DistanceCameraLineTrace::DistanceCameraLineTrace(Robot& _robot, double _targetDistance,
                                                 double _targetSpeed, int _targetPoint,
                                                 const PidGain& _pidGain,
                                                 ImageProcessor& _imageProcessor)
  : CameraPidTracking(_robot, _targetSpeed, _targetPoint, _pidGain, _imageProcessor),
    targetDistance(_targetDistance)
{
}

// 距離ライントレースの事前条件
bool DistanceCameraLineTrace::isMetPreCondition()
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
void DistanceCameraLineTrace::prepare()
{
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());
}

// 距離ライントレースの継続条件
bool DistanceCameraLineTrace::isMetContinuationCondition()
{
  // 走行距離が目標距離に到達
  if(fabs(Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                    robot.getMotorControllerInstance().getLeftMotorCount())
          - initDistance)
     >= targetDistance)
    return false;

  return true;
}