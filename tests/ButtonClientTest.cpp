/**
 * @file   ButtonClientTest.cpp
 * @brief  ButtonClientクラスのテスト
 * @author takuchi17
 */

#include "gtest/gtest.h"
#include "ButtonClient.h"

namespace etrobocon2025_test {

  // isRightPressedのテスト:
  // 右ボタンが押された状態と押されていない状態を正しく検出できることを検証する。
  TEST(ButtonClientTest, isRightPressed)
  {
    SpikeClient dummy_client;
    ButtonClient button_client(dummy_client);

    dummy_client.setButtonPressed(spike::ButtonTarget::RIGHT, true);
    EXPECT_TRUE(button_client.isRightPressed());

    dummy_client.setButtonPressed(spike::ButtonTarget::RIGHT, false);
    EXPECT_FALSE(button_client.isRightPressed());
  }

  // isLeftPressedのテスト:
  // 左ボタンが押された状態と押されていない状態を正しく検出できることを検証する。
  TEST(ButtonClientTest, isLeftPressed)
  {
    SpikeClient dummy_client;
    ButtonClient button_client(dummy_client);

    dummy_client.setButtonPressed(spike::ButtonTarget::LEFT, true);
    EXPECT_TRUE(button_client.isLeftPressed());

    dummy_client.setButtonPressed(spike::ButtonTarget::LEFT, false);
    EXPECT_FALSE(button_client.isLeftPressed());
  }

  // isCenterPressedのテスト:
  // 中央ボタンが押された状態と押されていない状態を正しく検出できることを検証する。
  TEST(ButtonClientTest, isCenterPressed)
  {
    SpikeClient dummy_client;
    ButtonClient button_client(dummy_client);

    dummy_client.setButtonPressed(spike::ButtonTarget::CENTER, true);
    EXPECT_TRUE(button_client.isCenterPressed());

    dummy_client.setButtonPressed(spike::ButtonTarget::CENTER, false);
    EXPECT_FALSE(button_client.isCenterPressed());
  }

  // MixedPressのテスト:
  // 複数のボタンが同時に押されたり、押されなかったりする状態を正しく検出できることを検証する。
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
