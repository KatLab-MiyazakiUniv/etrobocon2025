/**
 * @file   ColorLineTrace.cpp
 * @brief  色指定ライントレース動作
 * @author miyahara046 nishijima515
 */

#include "ColorLineTrace.h"

ColorLineTrace::ColorLineTrace(Robot& _robot, COLOR _targetColor, double _targetSpeed,
                               int _targetBrightness, const PidGain& _pidGain, bool& _isLeftEdge)
  : LineTrace(_robot, _targetSpeed, _targetBrightness, _pidGain, _isLeftEdge),
    targetColor(_targetColor),
    colorCount(0)
{
}

// 色指定ライントレースの事前条件
bool ColorLineTrace::isMetPreCondition()
{
  // targetColorがNONEのとき動作しない
  if(targetColor == COLOR::NONE) {
    return false;
  }

  // targetSpeedが0の場合は動作しない
  if(targetSpeed == 0.0) {
    return false;
  }
  return true;
}

// 色指定ライントレースの事前処理
void ColorLineTrace::prepare()
{
  // 初期値を代入
  colorCount = 0;
}

// 色指定ライントレースの継続条件
bool ColorLineTrace::isMetContinuationCondition()
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

  // 一定回数連続して一致したら終了
  if(colorCount >= JUDGE_COUNT) {
    return false;
  }

  return true;
}
