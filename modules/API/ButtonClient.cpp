/**
 * @file   ButtonClient.cpp
 * @brief  ボタン操作のためのクライアントAPIの実装
 * @author takuchi17
 */

#include "ButtonClient.h"
#include <string>

ButtonClient::ButtonClient(SpikeClient& client) : APIClient(client) {}

bool ButtonClient::isRightPressed()
{
  auto res = spikeClient.executeQuery<bool>(spike::CommandId::BUTTON_IS_PRESSED,
                                            spike::ButtonTarget::RIGHT);
  return res.value_or(false);
}

bool ButtonClient::isLeftPressed()
{
  auto res = spikeClient.executeQuery<bool>(spike::CommandId::BUTTON_IS_PRESSED,
                                            spike::ButtonTarget::LEFT);
  return res.value_or(false);
}

bool ButtonClient::isCenterPressed()
{
  auto res = spikeClient.executeQuery<bool>(spike::CommandId::BUTTON_IS_PRESSED,
                                            spike::ButtonTarget::CENTER);
  return res.value_or(false);
}