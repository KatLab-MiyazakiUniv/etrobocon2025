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

  TEST_F(ClockClientTest, sleep)
  {
    uint64_t sleep_duration = 1000;
    clock_client_.sleep(sleep_duration);
    EXPECT_EQ(dummy_client_.getMockTime(), sleep_duration);

    clock_client_.sleep(500);
    EXPECT_EQ(dummy_client_.getMockTime(), sleep_duration + 500);
  }

  TEST_F(ClockClientTest, now)
  {
    // Initially, time should be 0
    EXPECT_EQ(clock_client_.now(), 0);

    // After sleeping, now() should return the updated time
    uint64_t sleep_duration = 2000;
    clock_client_.sleep(sleep_duration);
    EXPECT_EQ(clock_client_.now(), sleep_duration);
  }

}  // namespace etrobocon2025_test
