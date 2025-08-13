#include "SpikeClient.h"

SpikeClient::SpikeClient() {}

bool SpikeClient::connect(const std::string& host, int port)
{
  return socket.connect(host, port);
}

void SpikeClient::disconnect()
{
  socket.close();
}