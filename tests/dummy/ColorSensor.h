/**
 * @file ColorSensor.h
 * @brief カラーセンサークラス（ダミー）
 * @author HaruArima08
 */

#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include "Port.h"
#include <stdlib.h>
#include <cstdint>

// ダミーのカラーID定数定義
#define PBIO_COLOR_HUE_RED 0
#define PBIO_COLOR_HUE_YELLOW 60
#define PBIO_COLOR_HUE_GREEN 120
#define PBIO_COLOR_HUE_BLUE 240

#define REFLECTION_BLACK 20  // ダミーの黒の反射率
#define REFLECTION_WHITE 90  // ダミーの白の反射率

namespace spikeapi {

  // カラーセンサークラス
  class ColorSensor {
   public:
    struct HSV {
      uint16_t h;
      uint8_t s;
      uint8_t v;
    };
    /**
     * コンストラクタ
     * @param port カラーセンサーポート番号
     * @return -
     */
    explicit ColorSensor(EPort port)
    {
      // 実際のセンサーではポートの初期化などを行う
      (void)port;  // 引数を使わないことを明示
    }

    /**
     * HSV値を取得
     * @param hsv HSV値を代入する変数（参照渡し）
     * @return HSVを保持するクラス
     */
    void getColor(HSV& hsv, bool surface = true)
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

    void getHSV(HSV& hsv, bool surface = true)
    {
      // getColorと同じ挙動をする
      getColor(hsv, surface);
    }

    /**
     * 反射率を取得
     * @return ダミーの反射率（黒か白の輝度どれか）
     */
    int32_t getReflection() { return (callCount++ == 0) ? REFLECTION_BLACK : REFLECTION_WHITE; }

   private:
    int callCount = 0;  // 黒、白の交互出力カウント
  };
}  // namespace spikeapi

#endif