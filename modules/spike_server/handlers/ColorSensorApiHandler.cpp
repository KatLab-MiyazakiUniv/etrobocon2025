#include "ColorSensorApiHandler.h"
#include <iostream>

ColorSensorApiHandler::ColorSensorApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    colorSensor_(EPort::PORT_E)
{
}

// send() is now inherited from ApiHandler

void ColorSensorApiHandler::handleGetReflection()
{
  spike::Int32Response response;
  response.value = -1;
  response.value = colorSensor_.getReflection();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void ColorSensorApiHandler::handleGetColorHsv()
{
  spike::HsvResponse response;
  response.h = -1.0f;
  response.s = -1.0f;
  response.v = -1.0f;
  spikeapi::ColorSensor::HSV hsv;
  colorSensor_.getColor(hsv);
  response.h = hsv.h;
  response.s = hsv.s;
  response.v = hsv.v;
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
