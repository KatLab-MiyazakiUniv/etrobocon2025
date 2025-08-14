/**
 * @file   ForceSensorClient.cpp
 * @brief  フォースセンサー操作のためのクライアントAPIの実装
 * @author takuchi17
 */

#include "ForceSensorClient.h"

ForceSensorClient::ForceSensorClient(SpikeClient& client) : APIClient(client) {}

bool ForceSensorClient::isPressed(float threshold)
{
  spike::ForceSensorIsPressedRequest req{ threshold };
  auto res = spikeClient.executeQuery<bool>(spike::CommandId::FORCE_SENSOR_IS_PRESSED, req);
  return res.value_or(false);
}

float ForceSensorClient::getForce()
{
  auto res = spikeClient.executeQuery<float>(spike::CommandId::FORCE_SENSOR_GET_FORCE);
  return res.value_or(0.0f);
}
