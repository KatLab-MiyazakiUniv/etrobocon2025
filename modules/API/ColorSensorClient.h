#ifndef COLORSENSOR_CLIENT_H
#define COLORSENSOR_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"

class ColorSensorClient {
 public:
  explicit ColorSensorClient(SpikeClient& client);
  spike::HsvResponse getColor();
  int32_t getReflection();

 private:
  SpikeClient& spikeClient;
};

#endif
