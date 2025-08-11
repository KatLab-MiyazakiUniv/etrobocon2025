#include "ButtonClient.h"
#include <string>

ButtonClient::ButtonClient(SpikeClient& client) : spikeClient(client) {}

bool ButtonClient::isRightPressed()
{
  std::string response = spikeClient.sendCommand("button,is_right_pressed");
  return response == "true";
}

bool ButtonClient::isLeftPressed()
{
  std::string response = spikeClient.sendCommand("button,is_left_pressed");
  return response == "true";
}

bool ButtonClient::isCenterPressed()
{
  std::string response = spikeClient.sendCommand("button,is_center_pressed");
  return response == "true";
}