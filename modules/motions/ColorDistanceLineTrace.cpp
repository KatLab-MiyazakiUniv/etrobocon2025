/**
 * @file   ColorDistanceLineTrace.cpp
 * @brief  色距離指定ライントレース動作
 * @author miyahara046 nishijima515
 */

#include "ColorDistanceLineTrace.h"

ColorDistanceLineTrace::ColorDistanceLineTrace(Robot& _robot, COLOR _targetColor,
                                               double _targetDistance, double _targetSpeed,
                                               int _targetBrightness, const PidGain& _pidGain,
                                               double _alpha)
  : LineTrace(_robot, _targetSpeed, _targetBrightness, _pidGain, _alpha),
    targetColor(_targetColor),
    targetDistance(_targetDistance),
    colorCount(0)
{
}
ColorDistanceLineTrace::ColorDistanceLineTrace(Robot& _robot, COLOR _targetColor,
                                               double _targetDistance, double _targetSpeed,
                                               int _targetBrightness, const PidGain& _pidGain)
  : ColorDistanceLineTrace(_robot, _targetColor, _targetDistance, _targetSpeed, _targetBrightness,
                           _pidGain, 0.8)  // デフォルトのalpha値を0.8に設定
{
}

// 色距離指定ライントレースの事前条件
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

  // targetDistance値が0以下の場合は終了する
  if(targetDistance <= 0.0) {
    return false;
  }

  return true;
}

// 色距離指定ライントレースの事前処理
void ColorDistanceLineTrace::prepare()
{
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());

  // 色カウントを初期化
  colorCount = 0;
}

bool ColorDistanceLineTrace::isMetContinuationCondition()
{
  // HSV値を取得
  spikeapi::ColorSensor::HSV hsv;
  robot.getColorSensorInstance().getColor(hsv);

  // 現在の色が目標色と一致していればカウント増加、違えばリセット
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
