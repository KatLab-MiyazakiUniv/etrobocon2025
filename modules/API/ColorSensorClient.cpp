#include "ColorSensorClient.h"
#include "../common/StringOperator.h"
#include <string>
#include <vector>

ColorSensorClient::ColorSensorClient(SpikeClient& client) : spikeClient(client) {}

void ColorSensorClient::getColor(spikeapi::ColorSensor::HSV& hsv)
{
  std::string response = spikeClient.sendCommand("colorsensor,get_color_hsv");
  std::vector<std::string> values = StringOperator::split(response, ',');
  if(values.size() == 3) {
    hsv.h = std::stof(values[0]);
    hsv.s = std::stof(values[1]);
    hsv.v = std::stof(values[2]);
  }
}

int ColorSensorClient::getReflection()
{
  std::string response = spikeClient.sendCommand("colorsensor,get_reflection");
  return std::stoi(response);
}
