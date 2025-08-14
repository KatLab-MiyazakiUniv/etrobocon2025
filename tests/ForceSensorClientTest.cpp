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

  // isPressedメソッドのテスト:
  // フォースセンサーのボタンが押された状態と押されていない状態を正しく検出できることを検証する。
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

  // getForceメソッドのテスト: フォースセンサーが測定した力が正しく取得できることを検証する。
  TEST_F(ForceSensorClientTest, getForce)
  {
    // Test default value
    EXPECT_FLOAT_EQ(0.0f, force_sensor_client_.getForce());

    // Set a new force value and test
    dummy_client_.setForce(5.25f);
    EXPECT_FLOAT_EQ(5.25f, force_sensor_client_.getForce());
  }

}  // namespace etrobocon2025_test
