/**
 * @file ColorJudge.cpp
 * @brief 色識別クラス
 * @author HaruArima08
 */

#include "ColorJudge.h"

COLOR ColorJudge::stringToColor(const char* str)
{
  if(strcmp(str, "BLACK") == 0) {  // 文字列がBLACKの場合
    return COLOR::BLACK;
  } else if(strcmp(str, "WHITE") == 0) {  // 文字列がWHITEの場合
    return COLOR::WHITE;
  } else if(strcmp(str, "BLUE") == 0) {  // 文字列がBLUEの場合
    return COLOR::BLUE;
  } else if(strcmp(str, "GREEN") == 0) {  // 文字列がGREENの場合
    return COLOR::GREEN;
  } else if(strcmp(str, "YELLOW") == 0) {  // 文字列がYELLOWの場合
    return COLOR::YELLOW;
  } else if(strcmp(str, "RED") == 0) {  // 文字列がREDの場合
    return COLOR::RED;
  } else {  // 想定していない文字列が来た場合
    return COLOR::NONE;
  }
}

const char* ColorJudge::colorToString(COLOR color)
{
  if(color == COLOR::BLACK) {  // BLACKの場合
    return "BLACK";
  } else if(color == COLOR::WHITE) {  // WHITEの場合
    return "WHITE";
  } else if(color == COLOR::BLUE) {  // BLUEの場合
    return "BLUE";
  } else if(color == COLOR::GREEN) {  // GREENの場合
    return "GREEN";
  } else if(color == COLOR::YELLOW) {  // YELLOWの場合
    return "YELLOW";
  } else if(color == COLOR::RED) {  // REDの場合
    return "RED";
  } else {  // 想定していない色の場合
    return "NONE";
  }
}

COLOR ColorJudge::getColor(const spikeapi::ColorSensor::HSV& hsv, bool surface)
{
  if(hsv.h == 0 && hsv.s == 0 && hsv.v == 0) {
    return COLOR::BLACK;  // BLACKの場合
  } else if(hsv.s == 0) {
    return COLOR::WHITE;  // WHITEの場合
  } else if(hsv.h == PBIO_COLOR_HUE_RED) {
    return COLOR::RED;  // REDの場合
  } else if(hsv.h == PBIO_COLOR_HUE_YELLOW) {
    return COLOR::YELLOW;  // YELLOWの場合
  } else if(hsv.h == PBIO_COLOR_HUE_GREEN) {
    return COLOR::GREEN;  // GREENの場合
  } else if(hsv.h == PBIO_COLOR_HUE_BLUE) {
    return COLOR::BLUE;  // BLUEの場合
  } else {
    return COLOR::NONE;  // 想定していない色の場合
  }
}
