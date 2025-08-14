/**
 * @file   ButtonClientTest.cpp
 * @brief  ButtonClientクラスのテスト
 * @author takuchi17
 */

#include "gtest/gtest.h"
#include "ButtonClient.h"

namespace etrobocon2025_test {

  TEST(ButtonClientTest, isRightPressed)
  {
    SpikeClient dummy_client;
    ButtonClient button_client(dummy_client);

    dummy_client.setButtonPressed(spike::ButtonTarget::RIGHT, true);
    EXPECT_TRUE(button_client.isRightPressed());

    dummy_client.setButtonPressed(spike::ButtonTarget::RIGHT, false);
    EXPECT_FALSE(button_client.isRightPressed());
  }

  TEST(ButtonClientTest, isLeftPressed)
  {
    SpikeClient dummy_client;
    ButtonClient button_client(dummy_client);

    dummy_client.setButtonPressed(spike::ButtonTarget::LEFT, true);
    EXPECT_TRUE(button_client.isLeftPressed());

    dummy_client.setButtonPressed(spike::ButtonTarget::LEFT, false);
    EXPECT_FALSE(button_client.isLeftPressed());
  }

  TEST(ButtonClientTest, isCenterPressed)
  {
    SpikeClient dummy_client;
    ButtonClient button_client(dummy_client);

    dummy_client.setButtonPressed(spike::ButtonTarget::CENTER, true);
    EXPECT_TRUE(button_client.isCenterPressed());

    dummy_client.setButtonPressed(spike::ButtonTarget::CENTER, false);
    EXPECT_FALSE(button_client.isCenterPressed());
  }

  TEST(ButtonClientTest, MixedPress)
  {
    SpikeClient dummy_client;
    ButtonClient button_client(dummy_client);

    dummy_client.setButtonPressed(spike::ButtonTarget::RIGHT, true);
    dummy_client.setButtonPressed(spike::ButtonTarget::LEFT, false);
    dummy_client.setButtonPressed(spike::ButtonTarget::CENTER, true);

    EXPECT_TRUE(button_client.isRightPressed());
    EXPECT_FALSE(button_client.isLeftPressed());
    EXPECT_TRUE(button_client.isCenterPressed());
  }

}  // namespace etrobocon2025_test
