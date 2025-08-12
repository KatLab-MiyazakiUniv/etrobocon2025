#include "ButtonApiHandler.h"
#include <iostream>

ButtonApiHandler::ButtonApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    button_()
{
}

// send() is now inherited from ApiHandler

void ButtonApiHandler::handleIsPressed(const spike::ButtonIsPressedRequest& request)
{
  spike::BoolResponse response;
  response.value = false;
  if(request.target == spike::ButtonTarget::RIGHT) {
    response.value = button_.isRightPressed();
  } else if(request.target == spike::ButtonTarget::LEFT) {
    response.value = button_.isLeftPressed();
  } else if(request.target == spike::ButtonTarget::CENTER) {
    response.value = button_.isCenterPressed();
  } else {
    response.value = false;
    std::cerr << "Error: Unknown ButtonTarget for isPressed" << std::endl;
  }
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
