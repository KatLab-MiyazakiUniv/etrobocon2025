#include "SpikeClient.h"
#include <iostream>

SpikeClient::SpikeClient() {}

bool SpikeClient::connect(const std::string& host, int port)
{
  // Mock connection, always succeed for now
  return true;
}

void SpikeClient::disconnect()
{
  // Mock disconnection
}

void SpikeClient::setCommandHandler(spike::CommandId id, CommandHandler handler)
{
  commandHandlers_[id] = handler;
}

MockResponse SpikeClient::handleCommand(spike::CommandId id,
                                        const std::vector<char>& request_payload)
{
  if(commandHandlers_.count(id)) {
    return commandHandlers_[id](request_payload);
  } else {
    std::cerr << "Error: No handler registered for CommandId: " << static_cast<uint16_t>(id)
              << std::endl;
    return { false, {} };
  }
}
