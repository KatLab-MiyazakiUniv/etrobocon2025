#ifndef SPIKE_SERVER_H
#define SPIKE_SERVER_H

#include "spikeapi.h"
#include "SpikeCommand.h"  // Include SpikeCommand.h
#include "modules/spike_server/handlers/MotorApiHandler.h"
#include "modules/spike_server/handlers/ColorSensorApiHandler.h"
#include "modules/spike_server/handlers/ClockApiHandler.h"
#include "modules/spike_server/handlers/ButtonApiHandler.h"
#include "modules/spike_server/handlers/ForceSensorApiHandler.h"
#include "modules/spike_server/handlers/DisplayApiHandler.h"
#include <string>
#include <vector>  // For potential future use with variable-length data

class Socket;  // Forward declaration

class SpikeServer {
 public:
  static void start();  // This will now create a SpikeServer instance

 private:
  // Constructor to initialize API handlers
  SpikeServer(Socket* client);

  // Generic receive and send for raw data (non-static now)
  bool receive(Socket* client, char* buffer, size_t size);
  bool send(Socket* client, const char* buffer, size_t size);

  // Command handling (non-static now)
  void handle_command(spike::CommandId commandId, Socket* client);

  // Member API handlers
  MotorApiHandler motorHandler_;
  ColorSensorApiHandler colorSensorHandler_;
  ClockApiHandler clockHandler_;
  ButtonApiHandler buttonHandler_;
  ForceSensorApiHandler forceSensorHandler_;
  DisplayApiHandler displayHandler_;
};

#endif  // SPIKE_SERVER_H