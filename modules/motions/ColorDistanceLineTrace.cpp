/**
 * @file   ColorDistanceLineTrace.cpp
 * @brief  色距離指定ライントレース動作
 * @author miyahara046
 */

#include "ColorDistanceLineTrace.h"
using namespace std;

ColorDistanceLineTrace::ColorDistanceLineTrace(Robot& _robot, COLOR _targetColor,
                                               double _targetDistance, double _targetSpeed,
                                               int _targetBrightness, const PidGain& _pidGain,
                                               bool& _isLeftEdge)
  : LineTrace(_robot, _targetSpeed, _targetBrightness, _pidGain, _isLeftEdge),
    targetColor(_targetColor),
    targetDistance(_targetDistance)
{
}

bool ColorDistanceLineTrace::isMetPreCondition()
{
  // 目標の色がNoneのとき終了する
  if(targetColor == COLOR::NONE) {
    return false;
  }

  // targetSpeed値が0の場合は終了する
  if(targetSpeed == 0.0) {
    return false;
  }

  // targetDistance値が0の場合は終了する
  if(targetDistance == 0.0) {
    return false;
  }

  return true;
}
void ColorDistanceLineTrace::prepare()
{
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());

  // 呼び出し時の走行距離
  initLeftMileage
      = Mileage::calculateWheelMileage(robot.getMotorControllerInstance().getLeftMotorCount());
  initRightMileage
      = Mileage::calculateWheelMileage(robot.getMotorControllerInstance().getRightMotorCount());

  // 色カウントを初期化
  colorCount = 0;
}

bool ColorDistanceLineTrace::isMetContinuationCondition()
{
  // HSV値を取得
  spikeapi::ColorSensor::HSV hsv;
  robot.getColorSensorInstance().getColor(hsv);
  if(ColorJudge::convertHsvToColor(hsv) == targetColor) {
    colorCount++;
  } else {
    colorCount = 0;
  }

  // (走行距離が目標距離に到達)||(指定された色をJUDGE_COUNT回連続で取得したとき)モータが止まる
  if((fabs(Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                     robot.getMotorControllerInstance().getLeftMotorCount())
           - initDistance)
      >= targetDistance)
     || (colorCount >= JUDGE_COUNT))
    return false;

  return true;
}
