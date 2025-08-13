/**
 * @file   ClockClient.cpp
 * @brief  時刻操作のためのクライアントAPIの実装
 * @author takuchi17
 */

#include "ClockClient.h"

ClockClient::ClockClient(SpikeClient& client) : spikeClient(client) {}

void ClockClient::sleep(uint64_t microseconds)
{
  spike::ClockSleepRequest req{ microseconds };
  spikeClient.executeCommand(spike::CommandId::CLOCK_SLEEP, req);
}

uint64_t ClockClient::now()
{
  auto res = spikeClient.executeQuery<uint64_t>(spike::CommandId::CLOCK_NOW);
  return res.value_or(0);
}
