/**
 * @file ColorSensor.h
 * @brief カラーセンサクラス
 * @author HaruArima08
 */

#ifndef COLOR_SENSOR_H_
#define COLOR_SENSOR_H_

#include "spikeapi.h"
#include "spike/pup/colorsensor.h"
#include "Port.h"
/**
 * SPIKE カラーセンサクラス
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
   * コンストラクタ
   * @param port PUPポートID
   */
  ColorSensor(Port port)
  {
    pupDevicePointer = pup_color_sensor_get_device(static_cast<pbio_port_id_t>(port));
  }

  /**
   * カラーセンサのRGB値を取得する
   * @param  値を設定するRGB構造体、各色8ビット
   * @return -
   */
  void getRGB(RGB& rgb) const
  {
    pup_color_rgb_t pup_rgb = pup_color_sensor_rgb(pupDevicePointer);
    rgb.r = pup_rgb.r;
    rgb.g = pup_rgb.g;
    rgb.b = pup_rgb.b;
  }

  /**
   * カラーセンサで色を測定する
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
   * カラーセンサで色を測定する（近似なし）
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
   * センサーの発する光を表面がどの程度反射するかを測定する
   * @return どの程度反射しているか（％）
   */
  int32_t getReflection() const { return pup_color_sensor_reflection(pupDevicePointer); }

  /**
   * 周囲の光の強度を測定する
   * @return 周囲の光の強度（％）
   */
  int32_t getAmbient() const { return pup_color_sensor_ambient(pupDevicePointer); }

  /**
   * カラーセンサのライトを設定する
   * @param bv1 輝度1
   * @param bv2 輝度2
   * @param bv3 輝度3
   * @return -
   */
  void setLight(int32_t bv1, int32_t bv2, int32_t bv3) const
  {
    pup_color_sensor_light_set(pupDevicePointer, bv1, bv2, bv3);
  }

  /**
   * カラーセンサのライトを点灯する
   * @param -
   * @return -
   */
  void lightOn() const { pup_color_sensor_light_on(pupDevicePointer); }

  /**
   * カラーセンサのライトを消灯する
   * @param -
   * @return -
   */
  void lightOff() const { pup_color_sensor_light_off(pupDevicePointer); }

  /**
   * カラーセンサが検知する色を設定する
   * @param size カラーの配列のサイズ
   * @param colors カラーの配列
   * @return -
   */
  void setDetectableColors(int32_t size, pup_color_hsv_t* colors) const
  {
    pup_color_sensor_detectable_colors(size, colors);
  }

  /**
   * インスタンス生成が正常にできたかどうかを確認するための共通メソッド
   * pupDevicePointerがNULLの場合にtrueとなる
   */
  bool hasError() { return pupDevicePointer == 0; }

 private:
  pup_device_t* pupDevicePointer;
};  // class ColorSensor

#endif
