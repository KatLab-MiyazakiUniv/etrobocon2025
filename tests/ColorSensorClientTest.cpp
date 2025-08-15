/**
 * @file   ColorSensorClientTest.cpp
 * @brief  ColorSensorClientクラスのテスト
 * @author takuchi17
 */

#include "gtest/gtest.h"
#include "ColorSensorClient.h"
#include "SpikeClient.h"

namespace etrobocon2025_test {

  class ColorSensorClientTest : public ::testing::Test {
   protected:
    SpikeClient dummy_client_;
    ColorSensorClient color_sensor_client_{ dummy_client_ };
  };

  // getReflectionのテスト: 反射率が正しく取得できることを検証する。
  TEST_F(ColorSensorClientTest, getReflection)
  {
    // Test default value (which is 20, representing black)
    EXPECT_EQ(20, color_sensor_client_.getReflection());

    // Set a new reflection value and test
    dummy_client_.setReflection(95);  // White
    EXPECT_EQ(95, color_sensor_client_.getReflection());
  }

  // getColorのテスト: HSV値が正しく取得できることを検証する。
  TEST_F(ColorSensorClientTest, getColor)
  {
    // Test default value (black)
    spike::HsvResponse hsv = color_sensor_client_.getColor();
    EXPECT_EQ(0, hsv.h);
    EXPECT_EQ(0, hsv.s);
    EXPECT_EQ(0, hsv.v);

    // Set a new color (e.g., green) and test
    spike::HsvResponse green{ 120, 100, 100 };
    dummy_client_.setHsv(green);
    hsv = color_sensor_client_.getColor();
    EXPECT_EQ(green.h, hsv.h);
    EXPECT_EQ(green.s, hsv.s);
    EXPECT_EQ(green.v, hsv.v);
  }

}  // namespace etrobocon2025_test
