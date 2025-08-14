/**
 * @file   DisplayClientTest.cpp
 * @brief  DisplayClientクラスのテスト
 * @author takuchi17
 */

#include "gtest/gtest.h"
#include "DisplayClient.h"
#include "SpikeClient.h"

namespace etrobocon2025_test {

  class DisplayClientTest : public ::testing::Test {
   protected:
    SpikeClient dummy_client_;
    DisplayClient display_client_{ dummy_client_ };
  };

  // 実行できるかのテスト
  TEST_F(DisplayClientTest, showCharNoCrash)
  {
    ASSERT_NO_THROW(display_client_.showChar('A'));
  }

  TEST_F(DisplayClientTest, showNumberNoCrash)
  {
    ASSERT_NO_THROW(display_client_.showNumber(42));
  }

  TEST_F(DisplayClientTest, scrollTextNoCrash)
  {
    ASSERT_NO_THROW(display_client_.scrollText("Hello World"));
  }
}  // namespace etrobocon2025_test
