#ifndef CLOCK_API_HANDLER_H
#define CLOCK_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "Clock.h"

class ClockApiHandler : public ApiHandler {  // Inherit from ApiHandler
 public:
  explicit ClockApiHandler(Socket* client);

  void handleSleep(const spike::ClockSleepRequest& request);
  void handleNow();

 private:
  // client_ and send() are now inherited from ApiHandler
  spikeapi::Clock clock_;
};

#endif  // CLOCK_API_HANDLER_H
