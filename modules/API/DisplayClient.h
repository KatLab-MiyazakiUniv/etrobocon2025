#ifndef DISPLAY_CLIENT_H
#define DISPLAY_CLIENT_H

#include "../spike_client/SpikeClient.h"
#include <string>

class DisplayClient {
public:
    explicit DisplayClient(SpikeClient& client);
    void drawString(const char* str, int x, int y);

private:
    SpikeClient& spikeClient;
};

#endif
