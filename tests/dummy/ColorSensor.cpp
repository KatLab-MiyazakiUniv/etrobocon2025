/**
 * @file ColorSensor.cpp
 * @brief カラーセンサクラスで用いる関数（ダミー）
 * @author HaruArima08
 */

#include "ColorSensor.h"
using namespace spikeapi;

// コンストラクタ
ColorSensor::ColorSensor(EPort port) {}

// HSV値を取得
void ColorSensor::getColor(pup_color_hsv_t& hsv)
{
  int index = rand() % 6;
  switch(index) {
    case 0:
      hsv = { 0, 0, 100 };  // white
      break;
    case 1:
      hsv = { 0, 100, 100 };  // red
      break;
    case 2:
      hsv = { 60, 100, 100 };  // yellow
      break;
    case 3:
      hsv = { 120, 100, 100 };  // green
      break;
    case 4:
      hsv = { 240, 100, 100 };  // blue
      break;
    case 5:
      hsv = { 0, 0, 0 };  // none
      break;
  }
}