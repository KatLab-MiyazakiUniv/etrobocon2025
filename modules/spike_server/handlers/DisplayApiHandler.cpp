#include "DisplayApiHandler.h"
#include <iostream>

DisplayApiHandler::DisplayApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    display_()
{
}

// send() is now inherited from ApiHandler

void DisplayApiHandler::handleShowChar(const spike::DisplayShowCharRequest& request)
{
  spike::Response response;
  response.value = false;
  display_.showChar(request.character);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void DisplayApiHandler::handleShowNumber(const spike::DisplayShowNumberRequest& request)
{
  spike::Response response;
  response.value = false;
  display_.showNumber(request.number);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void DisplayApiHandler::handleScrollText(const spike::DisplayScrollTextRequest& request)
{
  spike::Response response;
  response.value = false;
  display_.scrollText(request.text, request.speed);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
