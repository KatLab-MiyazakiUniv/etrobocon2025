/**
 * @file   DisplayClient.cpp
 * @brief  ディスプレイ操作のためのクライアントAPIの実装
 * @author takuchi17
 */

#include "DisplayClient.h"
#include <string.h>

DisplayClient::DisplayClient(SpikeClient& client) : spikeClient(client) {}

void DisplayClient::showChar(const char str)
{
  spike::DisplayShowCharRequest req{ str };
  spikeClient.executeCommand(spike::CommandId::DISPLAY_SHOW_CHAR, req);
}

void DisplayClient::showNumber(const int8_t number)
{
  spike::DisplayShowNumberRequest req{ number };
  spikeClient.executeCommand(spike::CommandId::DISPLAY_SHOW_NUMBER, req);
}

void DisplayClient::scrollText(const char* text, uint32_t delay)
{
  spike::DisplayScrollTextRequest req;
  req.delay = delay;
  strncpy(req.text, text, sizeof(req.text) - 1);
  req.text[sizeof(req.text) - 1] = '\0';  // Ensure null termination

  spikeClient.executeCommand(spike::CommandId::DISPLAY_SCROLL_TEXT, req);
}
