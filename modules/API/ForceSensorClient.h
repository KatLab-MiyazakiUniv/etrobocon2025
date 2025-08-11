#ifndef FORCESENSOR_CLIENT_H
#define FORCESENSOR_CLIENT_H

#include "../spike_client/SpikeClient.h"

class ForceSensorClient {
public:
    explicit ForceSensorClient(SpikeClient& client);
    bool isPressed();
    int getForce();

private:
    SpikeClient& spikeClient;
};

#endif
