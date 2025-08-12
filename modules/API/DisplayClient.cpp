#include "DisplayClient.h"
#include <string.h>

DisplayClient::DisplayClient(SpikeClient& client) : spikeClient(client) {}

void DisplayClient::showChar(const char str) {
    spike::DisplayShowCharRequest req{ str };
    spikeClient.executeCommand(
        spike::CommandId::DISPLAY_SHOW_CHAR,
        req
    );
}

void DisplayClient::showNumber(const int8_t number) {
    spike::DisplayShowNumberRequest req{ number };
    spikeClient.executeCommand(
        spike::CommandId::DISPLAY_SHOW_NUMBER,
        req
    );
}

void DisplayClient::scrollText(const char* text, int speed) {
    spike::DisplayScrollTextRequest req;
    req.speed = speed;
    strncpy(req.text, text, sizeof(req.text) - 1);
    req.text[sizeof(req.text) - 1] = '\0'; // Ensure null termination

    spikeClient.executeCommand(
        spike::CommandId::DISPLAY_SCROLL_TEXT,
        req
    );
}
