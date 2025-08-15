/**
 * @file   ColorSensorApiHandler.cpp
 * @brief  カラーセンサーAPIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "ColorSensorApiHandler.h"
#include <iostream>

ColorSensorApiHandler::ColorSensorApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    colorSensor(EPort::PORT_E)
{
}

// send() is now inherited from ApiHandler

spike::Int32Response ColorSensorApiHandler::handleGetReflection()
{
  spike::Int32Response response;
  response.value = -1;
  response.value = colorSensor.getReflection();
  return response;
}

spike::HsvResponse ColorSensorApiHandler::handleGetColorHsv()
{
  spike::HsvResponse response;
  response.h = 0;
  response.s = 0;
  response.v = 0;
  spikeapi::ColorSensor::HSV hsv;
  colorSensor.getColor(hsv);
  response.h = hsv.h;
  response.s = hsv.s;
  response.v = hsv.v;
  return response;
}
