#ifndef ROBOT_STATE_CACHE_H
#define ROBOT_STATE_CACHE_H

#include "SpikeClient.h"
#include "SpikeCommand.h"
#include <thread>
#include <mutex>

namespace spike {
    // Forward declaration of AllRobotStateResponse if needed, but it's already in SpikeCommand.h
}

class RobotStateCache {
public:
    explicit RobotStateCache(SpikeClient& client);
    ~RobotStateCache();

    void startUpdating();
    void stopUpdating();

    // Getter methods for cached data
    spike::AllRobotStateResponse getCachedState();

private:
    void updateLoop();

    SpikeClient& spikeClient;
    spike::AllRobotStateResponse cachedState;
    std::thread updateThread;
    std::mutex mtx;
    bool running;
};

#endif // ROBOT_STATE_CACHE_H