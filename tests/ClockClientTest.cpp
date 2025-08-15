/**
 * @file   ClockClientTest.cpp
 * @brief  ClockClientクラスのテスト
 * @author takuchi17
 */

#include "gtest/gtest.h"
#include "ClockClient.h"
#include "SpikeClient.h"

namespace etrobocon2025_test {

  class ClockClientTest : public ::testing::Test {
   protected:
    SpikeClient dummy_client_;
    ClockClient clock_client_{ dummy_client_ };
  };

  // sleepメソッドのテスト: 指定した時間だけスリープし、内部の時間が正しく更新されることを検証する。
  TEST_F(ClockClientTest, sleep)
  {
    uint64_t sleep_duration = 1000;
    clock_client_.sleep(sleep_duration);
    EXPECT_EQ(sleep_duration, dummy_client_.getMockTime());

    clock_client_.sleep(500);
    EXPECT_EQ(sleep_duration + 500, dummy_client_.getMockTime());
  }

  // nowメソッドのテスト: 現在の時間が正しく取得できることを検証する。
  TEST_F(ClockClientTest, now)
  {
    // Initially, time should be 0
    EXPECT_EQ(0, clock_client_.now());

    // After sleeping, now() should return the updated time
    uint64_t sleep_duration = 2000;
    clock_client_.sleep(sleep_duration);
    EXPECT_EQ(sleep_duration, clock_client_.now());
  }

}  // namespace etrobocon2025_test
