/**
 * @file   ForceSensorApiHandler.cpp
 * @brief  フォースセンサーAPIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "ForceSensorApiHandler.h"
#include <iostream>

ForceSensorApiHandler::ForceSensorApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    forceSensor(EPort::PORT_D)
{
}

// send() is now inherited from ApiHandler

void ForceSensorApiHandler::handleIsPressed(const spike::ForceSensorIsPressedRequest& request)
{
  spike::BoolResponse response;
  response.value = false;
  response.value = forceSensor.isPressed(request.threshold);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void ForceSensorApiHandler::handleGetForce()
{
  spike::FloatResponse response;
  response.value = 0.0f;
  response.value = forceSensor.getForce();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
