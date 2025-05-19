/**
 * @file colorsensor.h
 * @brief カラーセンサークラス (ダミー)
 * @author HaruArima08
 */

#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <stdint.h>

typedef struct {
  uint16_t r;
  uint16_t g;
  uint16_t b;
} pup_color_rgb_t;

typedef struct {
  uint16_t h;
  uint8_t s;
  uint8_t v;
} pup_color_hsv_t;

typedef struct {
} pup_device_t;

// カラーセンサーのデバイス取得関数のダミー (常に同じダミーオブジェクトのポインタを返す)
inline pup_device_t* pup_color_sensor_get_device(int /*port*/)
{
  static pup_device_t dummy;
  return &dummy;
}

// カラーセンサーのRGB値取得関数のダミー
inline pup_color_rgb_t pup_color_sensor_rgb(pup_device_t* /*device*/)
{
  return { 32, 36, 40 };
}

// カラーセンサーの色取得関数のダミー（近似あり）
inline pup_color_hsv_t pup_color_sensor_color(pup_device_t* /*device*/, bool /*surface*/)
{
  return { 0, 0, 100 };
}

// カラーセンサーの色取得関数のダミー (近似なし)
inline pup_color_hsv_t pup_color_sensor_hsv(pup_device_t* /*device*/, bool /*surface*/)
{
  return { 0, 83, 111 };
}

// カラーセンサーの反射率取得関数のダミー
inline int32_t pup_color_sensor_reflection(pup_device_t* /*device*/)
{
  return 42;
}

// カラーセンサーの周囲光強度取得関数のダミー
inline int32_t pup_color_sensor_ambient(pup_device_t* /*device*/)
{
  return 10;
}

// カラーセンサーのライト設定関数のダミー
inline void pup_color_sensor_light_set(pup_device_t* /*device*/, int32_t, int32_t, int32_t) {}

// カラーセンサーのライト点灯関数のダミー
inline void pup_color_sensor_light_on(pup_device_t* /*device*/) {}

// カラーセンサーのライト消灯関数のダミー
inline void pup_color_sensor_light_off(pup_device_t* /*device*/) {}

// カラーセンサーの検出可能色取得関数のダミー
inline void pup_color_sensor_detectable_colors(int32_t /*size*/, pup_color_hsv_t* /*colors*/) {}

#endif  // COLOR_SENSOR_H
