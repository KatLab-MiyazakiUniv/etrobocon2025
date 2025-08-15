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

spike::BoolResponse ForceSensorApiHandler::handleIsPressed(const spike::ForceSensorIsPressedRequest& request)
{
  spike::BoolResponse response;
  response.value = false;
  response.value = forceSensor.isPressed(request.threshold);
  return response;
}

spike::FloatResponse ForceSensorApiHandler::handleGetForce()
{
  spike::FloatResponse response;
  response.value = 0.0f;
  response.value = forceSensor.getForce();
  return response;
}
