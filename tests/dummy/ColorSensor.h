/**
 * @file ColorSensor.h
 * @brief カラーセンサクラス（ダミー）
 * @author HaruArima08
 */

#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include "Port.h"
#include <stdlib.h>
#include <cstdint>

typedef struct {
  uint16_t h;  // 色相
  uint8_t s;   // 彩度
  uint8_t v;   // 明度
} pup_color_hsv_t;

// 型定義（Colorjudgeで使われる）
typedef int pbio_color_hue_t;

// ダミーのカラーID定数定義
#define PBIO_COLOR_HUE_RED 0
#define PBIO_COLOR_HUE_YELLOW 60
#define PBIO_COLOR_HUE_GREEN 120
#define PBIO_COLOR_HUE_BLUE 240

namespace spikeapi {

  // カラーセンサクラス
  class ColorSensor {
   public:
    struct HSV {
      uint16_t h;
      uint8_t s;
      uint8_t v;
    };
    /**
     * コンストラクタ
     * @param port カラーセンサポート番号
     * @return -
     */
    explicit ColorSensor(EPort port)
    {
      // 実際のセンサではポートの初期化などを行う
      (void)port;  // 引数を使わないことを明示
    }

    /**
     * HSV値を取得
     * @param hsv HSV値を代入する変数（参照渡し）
     * @return HSVを保持するクラス
     */
    void getColor(pup_color_hsv_t& hsv)
    {
      int index = rand() % 6;
      switch(index) {
        case 0:
          hsv = { 0, 0, 100 };  // white
          break;
        case 1:
          hsv = { PBIO_COLOR_HUE_RED, 100, 100 };  // red
          break;
        case 2:
          hsv = { PBIO_COLOR_HUE_YELLOW, 100, 100 };  // yellow
          break;
        case 3:
          hsv = { PBIO_COLOR_HUE_GREEN, 100, 100 };  // green
          break;
        case 4:
          hsv = { PBIO_COLOR_HUE_BLUE, 100, 100 };  // blue
          break;
        case 5:
          hsv = { 0, 0, 0 };  // black
          break;
        default:
          hsv = { 123, 0, 0 };  // none (hsv.hは未定義の値(例:123))
          break;
      }
    }
  };
}  // namespace spikeapi

#endif