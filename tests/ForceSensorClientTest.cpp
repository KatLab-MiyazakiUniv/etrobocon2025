/**
 * @file   ForceSensorClientTest.cpp
 * @brief  ForceSensorClientクラスのテスト
 * @author takuchi17
 */

#include "gtest/gtest.h"
#include "ForceSensorClient.h"
#include "SpikeClient.h"

namespace etrobocon2025_test {

  class ForceSensorClientTest : public ::testing::Test {
   protected:
    SpikeClient dummy_client_;
    ForceSensorClient force_sensor_client_{ dummy_client_ };
  };

  TEST_F(ForceSensorClientTest, isPressed)
  {
    // Test default value
    EXPECT_FALSE(force_sensor_client_.isPressed());

    // Set to true and test
    dummy_client_.setForceSensorPressed(true);
    EXPECT_TRUE(force_sensor_client_.isPressed());

    // Set to false and test
    dummy_client_.setForceSensorPressed(false);
    EXPECT_FALSE(force_sensor_client_.isPressed());
  }

  TEST_F(ForceSensorClientTest, getForce)
  {
    // Test default value
    EXPECT_FLOAT_EQ(force_sensor_client_.getForce(), 0.0f);

    // Set a new force value and test
    dummy_client_.setForce(5.25f);
    EXPECT_FLOAT_EQ(force_sensor_client_.getForce(), 5.25f);
  }

}  // namespace etrobocon2025_test
