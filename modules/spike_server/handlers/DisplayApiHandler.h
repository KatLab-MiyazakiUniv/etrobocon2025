#ifndef DISPLAY_API_HANDLER_H
#define DISPLAY_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "Display.h"

class DisplayApiHandler : public ApiHandler {  // Inherit from ApiHandler
 public:
  explicit DisplayApiHandler(Socket* client);

  void handleShowChar(const spike::DisplayShowCharRequest& request);
  void handleShowNumber(const spike::DisplayShowNumberRequest& request);
  void handleScrollText(const spike::DisplayScrollTextRequest& request);

 private:
  // client_ and send() are now inherited from ApiHandler
  spikeapi::Display display_;
};

#endif  // DISPLAY_API_HANDLER_H
