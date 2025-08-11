#include "ClockClient.h"
#include <string>

ClockClient::ClockClient(SpikeClient& client) : spikeClient(client) {}

void ClockClient::sleep(int time) {
    spikeClient.sendCommand("clock,sleep," + std::to_string(time));
}

unsigned long ClockClient::now() {
    std::string res = spikeClient.sendCommand("clock,now");
    return std::stoul(res);
}
