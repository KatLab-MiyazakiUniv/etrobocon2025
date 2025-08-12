#include "ClockApiHandler.h"
#include <iostream>

ClockApiHandler::ClockApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    clock_()
{
}

// send() is now inherited from ApiHandler

void ClockApiHandler::handleSleep(const spike::ClockSleepRequest& request)
{
  spike::Response response;
  response.value = false;
  clock_.sleep(request.milliseconds);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void ClockApiHandler::handleNow()
{
  spike::UInt32Response response;
  response.value = 0;
  response.value = clock_.now();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
