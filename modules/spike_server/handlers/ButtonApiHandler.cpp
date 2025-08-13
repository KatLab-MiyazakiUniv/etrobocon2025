/**
 * @file   ButtonApiHandler.cpp
 * @brief  ボタンAPIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "ButtonApiHandler.h"
#include <iostream>

ButtonApiHandler::ButtonApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    button()
{
}

// send() is now inherited from ApiHandler

void ButtonApiHandler::handleIsPressed(const spike::ButtonIsPressedRequest& request)
{
  spike::BoolResponse response;
  response.value = false;
  if(request.target == spike::ButtonTarget::RIGHT) {
    response.value = button.isRightPressed();
  } else if(request.target == spike::ButtonTarget::LEFT) {
    response.value = button.isLeftPressed();
  } else if(request.target == spike::ButtonTarget::CENTER) {
    response.value = button.isCenterPressed();
  } else {
    response.value = false;
    std::cerr << "Error: Unknown ButtonTarget for isPressed" << std::endl;
  }
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
