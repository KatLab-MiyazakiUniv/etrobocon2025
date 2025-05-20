/**
 * @file ColorMeasureTest.cpp
 * @brief ColorSensorクラスの値取得をテストする
 * @author HaruArima08
 */

#include "ColorSensor.h"
#include <gtest/gtest.h>

// pup_color_rgb_tの比較用関数
bool eqRgb(pup_color_rgb_t rgb1, pup_color_rgb_t rgb2)
{
  return rgb1.r == rgb2.r && rgb1.g == rgb2.g && rgb1.b == rgb2.b;
}

// pup_color_hsv_tの比較用関数
bool eqHsv(pup_color_hsv_t hsv1, pup_color_hsv_t hsv2)
{
  return hsv1.h == hsv2.h && hsv1.s == hsv2.s && hsv1.v == hsv2.v;
}

namespace etrobocon2025_test {

  // RGB値取得テスト
  TEST(RgbColorTest, getRGB)
  {
    ColorSensor colorSensor(Port::PORT_E);
    ColorSensor::RGB rgb;
    colorSensor.getRGB(rgb);
    pup_color_rgb_t actual = { rgb.r, rgb.g, rgb.b };

    // テスト用のRGB期待値の定義
    pup_color_rgb_t expectedColors[] = {
      { 32, 36, 40 },     // black
      { 416, 404, 584 },  // white
      { 444, 76, 76 },    // red
      { 480, 432, 284 },  // yellow
      { 16, 300, 140 },   // green
      { 324, 368, 576 }   // blue
    };

    bool match = false;
    for(const auto& expected : expectedColors) {
      if(eqRgb(actual, expected)) {
        match = true;
        break;
      }
    }

    ASSERT_TRUE(match) << "RGB値が期待値に一致しません: "
                       << "r=" << (int)actual.r << ", g=" << (int)actual.g
                       << ", b=" << (int)actual.b;
  }

  // HSV値取得テスト (近似あり)
  TEST(HsvColorApproxiTest, getColor)
  {
    ColorSensor colorSensor(Port::PORT_E);
    ColorSensor::HSV hsv1;
    colorSensor.getColor(hsv1);
    pup_color_hsv_t actual = { hsv1.h, hsv1.s, hsv1.v };

    // テスト用のHSV期待値の定義
    pup_color_hsv_t expectedColors[] = {
      { 0, 83, 111 },    // red
      { 45, 41, 120 },   // yellow
      { 150, 94, 75 },   // green
      { 226, 44, 144 },  // blue
      { 245, 45, 146 },  // white
    };

    bool match = false;
    for(const auto& expected : expectedColors) {
      if(eqHsv(actual, expected)) {
        match = true;
        break;
      }
    }

    ASSERT_TRUE(match) << "HSV値が期待値に一致しません: "
                       << "h=" << (int)actual.h << ", s=" << (int)actual.s
                       << ", v=" << (int)actual.v;
  }

  // HSV値取得テスト (近似なし)
  TEST(HsvColorTest, getHSV)
  {
    ColorSensor colorSensor(Port::PORT_E);
    ColorSensor::HSV hsv2;
    colorSensor.getHSV(hsv2);
    pup_color_hsv_t actual = { hsv2.h, hsv2.s, hsv2.v };

    // テスト用のHSV期待値の定義
    pup_color_hsv_t expectedColors[] = {
      { 210, 20, 10 },   // black
      { 245, 45, 146 },  // white
      { 0, 83, 111 },    // red
      { 45, 41, 120 },   // yellow
      { 150, 94, 75 },   // green
      { 226, 44, 144 }   // blue
    };

    bool match = false;
    for(const auto& expected : expectedColors) {
      if(eqHsv(actual, expected)) {
        match = true;
        break;
      }
    }

    ASSERT_TRUE(match) << "HSV値が期待値に一致しません: "
                       << "h=" << (int)actual.h << ", s=" << (int)actual.s
                       << ", v=" << (int)actual.v;
  }

  // 反射率取得テスト
  TEST(ReflectionTest, getReflection)
  {
    ColorSensor colorSensor(Port::PORT_E);
    int32_t actual = colorSensor.getReflection();
    int32_t expected = 42;  // ダミーの期待値

    ASSERT_EQ(actual, expected) << "反射値が期待値に一致しません: " << actual;
  }

}  // namespace etrobocon2025_test
