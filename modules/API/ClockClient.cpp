#include "ClockClient.h"

ClockClient::ClockClient(SpikeClient& client) : spikeClient(client) {}

void ClockClient::sleep(uint32_t milliseconds) {
    spike::ClockSleepRequest req{ milliseconds };
    spikeClient.executeCommand(
        spike::CommandId::CLOCK_SLEEP,
        req
    );
}

uint32_t ClockClient::now() {
    auto res = spikeClient.executeQuery<uint32_t>(
        spike::CommandId::CLOCK_NOW
    );
    return res.value_or(0);
}
