#include "ForceSensorApiHandler.h"
#include <iostream>

ForceSensorApiHandler::ForceSensorApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    forceSensor_(EPort::PORT_D)
{
}

// send() is now inherited from ApiHandler

void ForceSensorApiHandler::handleIsPressed(const spike::ForceSensorIsPressedRequest& request)
{
  spike::BoolResponse response;
  response.value = false;
  response.value = forceSensor_.isPressed(request.threshold);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void ForceSensorApiHandler::handleGetForce()
{
  spike::DoubleResponse response;
  response.value = false;
  response.value = forceSensor_.getForce();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
