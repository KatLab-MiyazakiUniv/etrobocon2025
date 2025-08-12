#ifndef FORCE_SENSOR_API_HANDLER_H
#define FORCE_SENSOR_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "ForceSensor.h"

class ForceSensorApiHandler : public ApiHandler {  // Inherit from ApiHandler
 public:
  explicit ForceSensorApiHandler(Socket* client);

  void handleIsPressed(const spike::ForceSensorIsPressedRequest& request);
  void handleGetForce();

 private:
  // client_ and send() are now inherited from ApiHandler
  spikeapi::ForceSensor forceSensor_;
};

#endif  // FORCE_SENSOR_API_HANDLER_H
