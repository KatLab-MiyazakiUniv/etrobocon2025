/**
 * @file   ColorStraight.cpp
 * @brief  目標色まで直進するクラス
 * @author HaruArima08
 */

#include "ColorStraight.h"

ColorStraight::ColorStraight(Robot& _robot, COLOR _targetColor, double _targetSpeed)
  : Straight(_robot, _targetSpeed), targetColor(_targetColor)
{
}

bool ColorStraight::isMetPreCondition()
{
  // 目標速度が0の場合は終了する
  if(targetSpeed == 0.0) {
    return false;
  }
  // 目標の色がNoneの場合は終了する
  if(targetColor == COLOR::NONE) {
    return false;
  }

  return true;
}

void ColorStraight::prepare()
{
  colorCount = 0;
}

bool ColorStraight::isMetContinuationCondition()
{
  // HSV値を取得
  spike::HsvResponse hsv = robot.getColorSensorInstance().getColor();
  if(ColorJudge::convertHsvToColor(hsv) == targetColor) {
    colorCount++;
  } else {
    colorCount = 0;
  }
  // 指定された色をJUDGE_COUNT回連続で取得したときモータが止まる
  if(colorCount >= JUDGE_COUNT) return false;

  return true;
}
