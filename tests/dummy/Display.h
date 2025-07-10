/**
 * @file Display.h
 * @brief ディスプレイクラス(ダミー)
 * @author HaruArima08 nishijima515
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>

namespace spikeapi {

  class Display {
   public:
    /**
     * コンストラクタ
     */
    Display(void) = default;

    /**
     * 数字を表示する
     * param num -99〜99 負の符号はディスプレイの中央の薄い点として表現される
     * return -
     */
    void showNumber(const int8_t num) {}

    /**
     * ハブ内蔵ディスプレイを消灯する
     * param  -
     * return -
     */
    void off() {}

    /**
     * イメージの行列(5x5)で表示する
     * param image 輝度の行列。２次元配列でも良い
     * return -
     */
    void setImage(uint8_t* image) {}

    /**
     * 1文字あるいは記号を表示する
     * param c
     * return -
     */
    void showChar(const char c) {}

    /**
     * 文字列をスクロールして表示する。
     * param text 表示する文字列
     * param delay スクロールの感覚(ms)
     * return -
     */
    void scrollText(const char* text, uint32_t delay) {}
  };

}  // namespace spikeapi
#endif