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

void ColorSensorApiHandler::handleGetReflection()
{
  spike::Int32Response response;
  response.value = -1;
  response.value = colorSensor.getReflection();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void ColorSensorApiHandler::handleGetColorHsv()
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
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
