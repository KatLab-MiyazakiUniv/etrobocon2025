#ifndef SPIKE_CLIENT_H
#define SPIKE_CLIENT_H

#include "../utils/Socket.h"
#include <string>
#include <vector>

class SpikeClient {
public:
    SpikeClient();
    bool connect(const std::string& host, int port);
    void disconnect();
    std::string sendCommand(const std::string& command);

private:
    Socket socket;
};

#endif
