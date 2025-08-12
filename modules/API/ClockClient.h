#ifndef CLOCK_CLIENT_H
#define CLOCK_CLIENT_H

#include <cstdint>
#include "SpikeClient.h"
#include "SpikeCommand.h"

class ClockClient {
public:
    explicit ClockClient(SpikeClient& client);
    void sleep(uint32_t milliseconds);
    uint32_t now();

private:
    SpikeClient& spikeClient;
};

#endif
