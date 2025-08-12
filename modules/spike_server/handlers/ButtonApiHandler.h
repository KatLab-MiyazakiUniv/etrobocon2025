#ifndef BUTTON_API_HANDLER_H
#define BUTTON_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "Button.h"

class ButtonApiHandler : public ApiHandler {  // Inherit from ApiHandler
 public:
  explicit ButtonApiHandler(Socket* client);

  void handleIsPressed(const spike::ButtonIsPressedRequest& request);

 private:
  // client_ and send() are now inherited from ApiHandler
  spikeapi::Button button_;
};

#endif  // BUTTON_API_HANDLER_H
