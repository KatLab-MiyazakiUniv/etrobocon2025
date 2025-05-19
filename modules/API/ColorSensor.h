/**
 * @file ColorSensor.h
 * @brief カラーセンサークラス (ラッパクラス)
 * @author HaruArima08
 */

#ifndef COLOR_SENSOR_H_
#define COLOR_SENSOR_H_

#include "spikeapi.h"
#include "spike/pup/colorsensor.h"
#include "Port.h"

/**
 * SPIKE カラーセンサークラス
 */
class ColorSensor {
 public:
  struct RGB {
    uint16_t r;
    uint16_t g;
    uint16_t b;
  };

  struct HSV {
    uint16_t h;
    uint8_t s;
    uint8_t v;
  };

  /**
   * @brief コンストラクタ
   * @param port PUPポートID
   */
  ColorSensor(Port port)
  {
    pupDevicePointer = pup_color_sensor_get_device(static_cast<pbio_port_id_t>(port));
  }

  /**
   * @brief カラーセンサーのRGB値を取得する
   * @param  rgb値を設定するRGB構造体、各色10ビット
   * @return 色（rgbによる表現）
   */
  void getRGB(RGB& rgb) const
  {
    pup_color_rgb_t pup_rgb = pup_color_sensor_rgb(pupDevicePointer);
    rgb.r = pup_rgb.r;
    rgb.g = pup_rgb.g;
    rgb.b = pup_rgb.b;
  }

  /**
   * @brief カラーセンサーで色を測定する
   * @param hsv値を設定するHSV構造体、h(16ビット)、s(8ビット)、v(8ビット)
   * @param surface trueならば表面の色から、falseならば他の光源の色を検出する
   * @return 色（hsvによる表現）
   */
  void getColor(HSV& hsv, bool surface = true) const
  {
    pup_color_hsv_t pup_hsv = pup_color_sensor_color(pupDevicePointer, surface);
    hsv.h = pup_hsv.h;
    hsv.s = pup_hsv.s;
    hsv.v = pup_hsv.v;
  }

  /**
   * @brief カラーセンサーで色を測定する（近似なし）
   * @param hsv値を設定するHSV構造体、h(16ビット)、s(8ビット)、v(8ビット)
   * @param surface trueならば表面の色から、falseならば他の光源の色を検出する
   * @return 色（hsvによる表現）
   */
  void getHSV(HSV& hsv, bool surface = true) const
  {
    pup_color_hsv_t pup_hsv = pup_color_sensor_hsv(pupDevicePointer, surface);
    hsv.h = pup_hsv.h;
    hsv.s = pup_hsv.s;
    hsv.v = pup_hsv.v;
  }

  /**
   * @brief センサーの発する光を表面がどの程度反射するかを測定する
   * @return どの程度反射しているか（％）
   */
  int32_t getReflection() const { return pup_color_sensor_reflection(pupDevicePointer); }

  /**
   * @brief 周囲の光の強度を測定する
   * @return 周囲の光の強度（％）
   */
  int32_t getAmbient() const { return pup_color_sensor_ambient(pupDevicePointer); }

  /**
   * @brief カラーセンサーのライトを設定する
   * @param bv1 輝度1 （明るさ0-100）
   * @param bv2 輝度2 （明るさ0-100）
   * @param bv3 輝度3 （明るさ0-100）
   * @return void
   */
  void setLight(int32_t bv1, int32_t bv2, int32_t bv3) const
  {
    pup_color_sensor_light_set(pupDevicePointer, bv1, bv2, bv3);
  }

  /**
   * @brief カラーセンサーのライトを点灯する
   * @return void
   */
  void lightOn() const { pup_color_sensor_light_on(pupDevicePointer); }

  /**
   * @brief カラーセンサーのライトを消灯する
   * @return void
   */
  void lightOff() const { pup_color_sensor_light_off(pupDevicePointer); }

  /**
   * @brief カラーセンサーが検知する色を設定する
   * @param size カラーの配列のサイズ
   * @param colors カラーの配列
   * @return void
   */
  void setDetectableColors(int32_t size, pup_color_hsv_t* colors) const
  {
    pup_color_sensor_detectable_colors(size, colors);
  }

  /**
   * @brief インスタンス生成が正常にできたかどうかを確認するための共通メソッド
   * @return pupDevicePointerがNULLの場合にtrueとなる
   */
  bool hasError() { return pupDevicePointer == 0; }

 private:
  pup_device_t* pupDevicePointer;
};  // class ColorSensor

#endif
