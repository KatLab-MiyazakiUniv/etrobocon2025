#ifndef COLOR_SENSOR_API_HANDLER_H
#define COLOR_SENSOR_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "ColorSensor.h"

class ColorSensorApiHandler : public ApiHandler {  // Inherit from ApiHandler
 public:
  explicit ColorSensorApiHandler(Socket* client);

  void handleGetReflection();
  void handleGetColorHsv();

 private:
  // client_ and send() are now inherited from ApiHandler
  spikeapi::ColorSensor colorSensor_;
};

#endif  // COLOR_SENSOR_API_HANDLER_H
