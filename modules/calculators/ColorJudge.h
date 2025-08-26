/**
 * @file ColorJudge.h
 * @brief 色識別クラス
 * @author HaruArima08
 */

#ifndef COLOR_JUDGE_H
#define COLOR_JUDGE_H

#include <string>
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
   * @param str 変換する文字列
   * @return 色
   */
  static COLOR convertStringToColor(const std::string& str);

  /**
   * @brief 列挙型COLORを文字列に変換する
   * @param color 色
   * @return 文字列の色
   */
  static const char* convertColorToString(const COLOR& color);

  /**
   * @brief カラーセンサーで色を測定する (近似あり)
   * @param hsv値を設定するHSV構造体、h(16ビット)、s(8ビット)、v(8ビット)
   * @return 色（hsvによる表現）
   */
  static COLOR convertHsvToColor(const spikeapi::ColorSensor::HSV& hsv);

 private:
  ColorJudge();
  static constexpr int SATURATION_BORDER = 33;    // 無彩色かどうかの彩度の境界
  static constexpr int BLACK_LIMIT_BORDER = 10;   // 黒の明度の境界
  static constexpr int WHITE_LIMIT_BORDER = 250;  // 白の明度の境界
  static constexpr int BLACK_BORDER = 95;         // 無彩色の黒の明度の境界
  static constexpr int RED_BORDER = 25;           // 赤の色相の境界
  static constexpr int YELLOW_BORDER = 50;        // 黄の色相の境界
  static constexpr int GREEN_BORDER = 170;        // 緑の色相の境界
  static constexpr int BLUE_BORDER = 300;         // 青の色相の境界
};

#endif