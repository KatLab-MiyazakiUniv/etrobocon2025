#include "DisplayClient.h"

DisplayClient::DisplayClient(SpikeClient& client) : spikeClient(client) {}

void DisplayClient::drawString(const char* str, int x, int y) {
    std::string command = "display,draw_string,";
    command += str;
    command += ",";
    command += std::to_string(x);
    command += ",";
    command += std::to_string(y);
    spikeClient.sendCommand(command);
}
