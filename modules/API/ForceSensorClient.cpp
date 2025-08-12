#include "ForceSensorClient.h"

ForceSensorClient::ForceSensorClient(SpikeClient& client) : spikeClient(client) {}

bool ForceSensorClient::isPressed(float threshold)
{
  spike::ForceSensorIsPressedRequest req{ threshold };
  auto res = spikeClient.executeQuery<bool>(spike::CommandId::FORCE_SENSOR_IS_PRESSED, req);
  return res.value_or(false);
}

int32_t ForceSensorClient::getForce()
{
  auto res = spikeClient.executeQuery<int32_t>(spike::CommandId::FORCE_SENSOR_GET_FORCE);
  return res.value_or(0);
}
