#ifndef DISPLAY_CLIENT_H
#define DISPLAY_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"
#include <string>

class DisplayClient {
 public:
  explicit DisplayClient(SpikeClient& client);
  void showChar(const char str);
  void showNumber(const int8_t number);
  void scrollText(const char* text, int speed = 50);

 private:
  SpikeClient& spikeClient;
};

#endif
