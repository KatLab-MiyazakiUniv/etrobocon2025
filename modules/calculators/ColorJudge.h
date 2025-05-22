/**
 * @file ColorJudge.h
 * @brief 色識別クラス
 * @author HaruArima08
 */

#ifndef COLOR_JUDGE_H
#define COLOR_JUDGE_H

#include <string.h>
#include <algorithm>
#include "ColorSensor.h"

enum class COLOR : int {
  NONE = 0,
  BLACK = 1,
  WHITE = 2,
  BLUE = 3,
  GREEN = 4,
  YELLOW = 5,
  RED = 6,
};

class ColorJudge {
 public:
  /**
   * @brief 文字列を列挙型COLORに変換する
   * @param str 文字列の色
   * @return 色
   */
  static COLOR stringToColor(const char* str);

  /**
   * @brief 列挙型COLORを文字列に変換する
   * @param color 色
   * @return 文字列の色
   */
  static const char* colorToString(COLOR color);

  /**
   * @brief カラーセンサで色を測定する
   * @param hsv値を設定するHSV構造体、h(16ビット)、s(8ビット)、v(8ビット)
   * @param surface trueならば表面の色から、falseならば他の光源の色を検出する
   * @return 色（hsvによる表現）
   */
  static COLOR getColor(const spikeapi::ColorSensor::HSV& hsv, bool surface = true);

 private:
  ColorJudge();  // 明示的にインスタンス化を禁止
};

#endif