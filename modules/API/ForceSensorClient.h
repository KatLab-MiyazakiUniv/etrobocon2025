#ifndef FORCESENSOR_CLIENT_H
#define FORCESENSOR_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"

class ForceSensorClient {
 public:
  explicit ForceSensorClient(SpikeClient& client);
  bool isPressed(float threshold = 0.5f);
  int32_t getForce();

 private:
  SpikeClient& spikeClient;
};

#endif
