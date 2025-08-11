#ifndef SPIKE_SERVER_H
#define SPIKE_SERVER_H

#include "spikeapi.h"
#include <string>

class Socket;  // Forward declaration

class SpikeServer {
 public:
  static void start();

 private:
  static void handle_command(const std::string& cmd, Socket* client);
};

#endif  // SPIKE_SERVER_H