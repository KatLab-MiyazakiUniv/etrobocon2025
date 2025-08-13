#ifndef CLOCK_CLIENT_H
#define CLOCK_CLIENT_H

#include <cstdint>
#include "SpikeClient.h"
#include "SpikeCommand.h"

class ClockClient {
 public:
  explicit ClockClient(SpikeClient& client);
  void sleep(uint64_t milliseconds);
  uint64_t now();

 private:
  SpikeClient& spikeClient;
};

#endif
