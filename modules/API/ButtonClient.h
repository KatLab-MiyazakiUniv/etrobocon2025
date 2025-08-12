#ifndef BUTTON_CLIENT_H
#define BUTTON_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"

class ButtonClient {
 public:
  explicit ButtonClient(SpikeClient& client);
  bool isRightPressed();
  bool isLeftPressed();
  bool isCenterPressed();

 private:
  SpikeClient& spikeClient;
};

#endif
