/**
 * @file   ColorLineTrace.cpp
 * @brief  指定色ライントレース動作
 * @author miyahara046
 */

#include "ColorLineTrace.h"

using namespace std;

ColorLineTrace::ColorLineTrace(Robot& _robot, COLOR _targetColor, double _targetSpeed,
                               int _targetBrightness, const PidGain& _pidGain, bool& _isLeftEdge)
  : LineTrace(_robot, _targetSpeed, _targetBrightness, _pidGain, _isLeftEdge),
    targetColor(_targetColor),
    colorCount(0)
{
}

// 色ライントレースの事前条件
bool ColorLineTrace::isMetPreCondition()
{
  // targetColorがNONEか、またはtargetSpeedが0.0のとき動作しない
  if(targetColor == COLOR::NONE || targetSpeed == 0.0) {
    return false;
  }

  return true;
}

void ColorLineTrace::prepare()
{
  // 初期値を代入
  colorCount = 0;
}

// 色ライントレースの継続条件
bool ColorLineTrace::isMetContinuationCondition()
{
  // 現在の色が目標色と一致していればカウント増加、違えばリセット
  if(ColorJudge::getColor(measurer.getRawColor()) == targetColor) {
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
