/**
 * @file   DisplayApiHandler.cpp
 * @brief  ディスプレイAPIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "DisplayApiHandler.h"
#include <iostream>

DisplayApiHandler::DisplayApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    display()
{
}

// send() is now inherited from ApiHandler

void DisplayApiHandler::handleShowChar(const spike::DisplayShowCharRequest& request)
{
  spike::Response response;
  response.value = false;
  display.showChar(request.character);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void DisplayApiHandler::handleShowNumber(const spike::DisplayShowNumberRequest& request)
{
  spike::Response response;
  response.value = false;
  display.showNumber(request.number);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void DisplayApiHandler::handleScrollText(const spike::DisplayScrollTextRequest& request)
{
  spike::Response response;
  response.value = false;
  display.scrollText(request.text, request.delay);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
