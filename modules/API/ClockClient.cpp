#include "ClockClient.h"

ClockClient::ClockClient(SpikeClient& client) : spikeClient(client) {}

void ClockClient::sleep(uint64_t milliseconds) {
    spike::ClockSleepRequest req{ milliseconds };
    spikeClient.executeCommand(
        spike::CommandId::CLOCK_SLEEP,
        req
    );
}

uint64_t ClockClient::now() {
    auto res = spikeClient.executeQuery<uint64_t>(
        spike::CommandId::CLOCK_NOW
    );
    return res.value_or(0);
}
