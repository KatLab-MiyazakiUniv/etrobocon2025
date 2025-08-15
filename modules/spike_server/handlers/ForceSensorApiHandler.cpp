/**
 * @file   ForceSensorApiHandler.cpp
 * @brief  フォースセンサーAPIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "ForceSensorApiHandler.h"
#include <iostream>
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>

ForceSensorApiHandler::ForceSensorApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    forceSensor(EPort::PORT_D)
{
}

// Helper function to convert float to network byte order
float htonf(float val)
{
  uint32_t rep;
  memcpy(&rep, &val, sizeof(rep));
  rep = htonl(rep);
  memcpy(&val, &rep, sizeof(rep));
  return val;
}

// Helper function to convert float from network byte order
float ntohf(float val)
{
  uint32_t rep;
  memcpy(&rep, &val, sizeof(rep));
  rep = ntohl(rep);
  memcpy(&val, &rep, sizeof(rep));
  return val;
}

// send() is now inherited from ApiHandler

void ForceSensorApiHandler::handleIsPressed(const spike::ForceSensorIsPressedRequest& request)
{
  spike::BoolResponse response;
  response.value = false;
  float threshold = ntohf(request.threshold);
  response.value = forceSensor.isPressed(threshold);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void ForceSensorApiHandler::handleGetForce()
{
  spike::FloatResponse response;
  response.value = 0.0f;
  response.value = forceSensor.getForce();
  response.value = htonf(response.value);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
