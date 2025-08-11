#include "ForceSensorClient.h"
#include <string>

ForceSensorClient::ForceSensorClient(SpikeClient& client) : spikeClient(client) {}

bool ForceSensorClient::isPressed() {
    std::string res = spikeClient.sendCommand("forcesensor,is_pressed");
    return res == "true";
}

int ForceSensorClient::getForce() {
    std::string res = spikeClient.sendCommand("forcesensor,get_force");
    return std::stoi(res);
}
