/**
 * @file ColorJudge.h
 * @brief 色識別クラス
 * @author HaruArima08
 */

#ifndef COLOR_JUDGE_H
#define COLOR_JUDGE_H

#include <string>
#include "../common/SpikeCommand.h"

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
  static COLOR convertHsvToColor(const spike::HsvResponse& hsv);

 private:
  /**
   * @brief コンストラクタ（インスタンス化禁止）
   */
  ColorJudge();
};

#endif