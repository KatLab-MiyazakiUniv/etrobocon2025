/**
 * @file Display.h
 * @brief ディスプレイクラス(ダミー)
 * @author HaruArima08
 */

#ifndef DISPLAY_H
#define DISPLAY_H

namespace spikeapi {

  class Display {
   public:
    /**
     * コンストラクタ
     */
    Display(void) = default;

    /**
     * ハブ内蔵ディスプレイを消灯する
     * param  -
     * return -
     */
    void off() {}

    /**
     * 1文字あるいは記号を表示する
     * param c
     * return -
     */
    void showChar(const char c) {}
  };

}  // namespace spikeapi
#endif