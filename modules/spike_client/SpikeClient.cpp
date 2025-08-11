#include "SpikeClient.h"

SpikeClient::SpikeClient() {}

bool SpikeClient::connect(const std::string& host, int port) {
    return socket.connect(host, port);
}

void SpikeClient::disconnect() {
    socket.close();
}

std::string SpikeClient::sendCommand(const std::string& command) {
    if (!socket.isValid()) {
        return "Error: Not connected";
    }

    if (!socket.send(command)) {
        return "Error: Failed to send command";
    }

    std::string response;
    if (socket.receive(response) <= 0) {
        return "Error: Failed to receive response";
    }

    return response;
}
