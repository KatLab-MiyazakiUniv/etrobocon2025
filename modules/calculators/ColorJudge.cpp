/**
 * @file ColorJudge.cpp
 * @brief 色識別クラス
 * @author HaruArima08
 */

#include "ColorJudge.h"

COLOR ColorJudge::convertStringToColor(const std::string& str)
{
  if(str == "BLACK") return COLOR::BLACK;
  if(str == "WHITE") return COLOR::WHITE;
  if(str == "BLUE") return COLOR::BLUE;
  if(str == "GREEN") return COLOR::GREEN;
  if(str == "YELLOW") return COLOR::YELLOW;
  if(str == "RED") return COLOR::RED;
  return COLOR::NONE;
}

const char* ColorJudge::convertColorToString(const COLOR& color)
{
  switch(color) {
    case COLOR::BLACK:
      return "BLACK";
    case COLOR::WHITE:
      return "WHITE";
    case COLOR::BLUE:
      return "BLUE";
    case COLOR::GREEN:
      return "GREEN";
    case COLOR::YELLOW:
      return "YELLOW";
    case COLOR::RED:
      return "RED";
    default:
      return "NONE";
  }
}

COLOR ColorJudge::convertHsvToColor(const spikeapi::ColorSensor::HSV& hsv)
{
  // HSV値がすべて0のときは黒
  if(hsv.h == 0 && hsv.s == 0 && hsv.v == 0) {
    return COLOR::BLACK;
  }
  // S値(彩度)が0のときは白
  if(hsv.s == 0) {
    return COLOR::WHITE;
  }
  // 定数定義されている色の判定はH値(色相)で行う
  switch(hsv.h) {
    case PBIO_COLOR_HUE_RED:
      return COLOR::RED;
    case PBIO_COLOR_HUE_YELLOW:
      return COLOR::YELLOW;
    case PBIO_COLOR_HUE_GREEN:
      return COLOR::GREEN;
    case PBIO_COLOR_HUE_BLUE:
      return COLOR::BLUE;
    default:
      break;
  }
  return COLOR::NONE;
}
