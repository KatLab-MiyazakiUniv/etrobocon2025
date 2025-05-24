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
    explicit ColorSensor(EPort port);

    /**
     * HSV値を取得
     * @param hsv HSV値を代入する変数（参照渡し）
     * @return HSVを保持するクラス
     */
    void getColor(pup_color_hsv_t& hsv);
  };
}  // namespace spikeapi

#endif