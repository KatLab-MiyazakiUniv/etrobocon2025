/**
 * @file   ColorSensorClient.cpp
 * @brief  カラーセンサー操作のためのクライアントAPIの実装
 * @author takuchi17
 */

#include "ColorSensorClient.h"

ColorSensorClient::ColorSensorClient(SpikeClient& client) : spikeClient(client) {}

spike::HsvResponse ColorSensorClient::getColor()
{
  auto res
      = spikeClient.executeQuery<spike::HsvResponse>(spike::CommandId::COLOR_SENSOR_GET_COLOR_HSV);
  return res.value_or(spike::HsvResponse{ 0, 0, 0 });
}

int32_t ColorSensorClient::getReflection()
{
  auto res = spikeClient.executeQuery<int32_t>(spike::CommandId::COLOR_SENSOR_GET_REFLECTION);
  return res.value_or(0);
}
