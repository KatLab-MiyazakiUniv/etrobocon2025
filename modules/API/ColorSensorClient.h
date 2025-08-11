#ifndef COLORSENSOR_CLIENT_H
#define COLORSENSOR_CLIENT_H

#include "../spike_client/SpikeClient.h"

class ColorSensorClient {
 public:
  explicit ColorSensorClient(SpikeClient& client);
  void getColor(HSV& hsv);
  int getReflection();

 private:
  SpikeClient& spikeClient;
};

#endif
