#ifndef CLOCK_CLIENT_H
#define CLOCK_CLIENT_H

#include "../spike_client/SpikeClient.h"

class ClockClient {
public:
    explicit ClockClient(SpikeClient& client);
    void sleep(int time);
    unsigned long now();

private:
    SpikeClient& spikeClient;
};

#endif
