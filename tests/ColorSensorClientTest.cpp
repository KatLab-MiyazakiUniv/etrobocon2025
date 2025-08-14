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

  TEST_F(ColorSensorClientTest, getReflection)
  {
    // Test default value (which is 20, representing black)
    EXPECT_EQ(color_sensor_client_.getReflection(), 20);

    // Set a new reflection value and test
    dummy_client_.setReflection(95);  // White
    EXPECT_EQ(color_sensor_client_.getReflection(), 95);
  }

  TEST_F(ColorSensorClientTest, getColor)
  {
    // Test default value (black)
    spike::HsvResponse hsv = color_sensor_client_.getColor();
    EXPECT_EQ(hsv.h, 0);
    EXPECT_EQ(hsv.s, 0);
    EXPECT_EQ(hsv.v, 0);

    // Set a new color (e.g., green) and test
    spike::HsvResponse green{ 120, 100, 100 };
    dummy_client_.setHsv(green);
    hsv = color_sensor_client_.getColor();
    EXPECT_EQ(hsv.h, green.h);
    EXPECT_EQ(hsv.s, green.s);
    EXPECT_EQ(hsv.v, green.v);
  }

}  // namespace etrobocon2025_test
