#include "RobotStateCache.h"
#include <chrono>
#include <iostream>  // For debugging

RobotStateCache::RobotStateCache(SpikeClient& client) : spikeClient(client), running(false)
{
  // Initialize cachedState with default values
  cachedState = {};
}

RobotStateCache::~RobotStateCache()
{
  stopUpdating();
}

void RobotStateCache::startUpdating()
{
  if(!running) {
    running = true;
    updateThread = std::thread(&RobotStateCache::updateLoop, this);
    std::cerr << "RobotStateCache: Update thread started." << std::endl;
  }
}

void RobotStateCache::stopUpdating()
{
  if(running) {
    running = false;
    std::cerr << "RobotStateCache: Stopping update thread..." << std::endl;
    if(updateThread.joinable()) {
      updateThread.join();
      std::cerr << "RobotStateCache: Update thread joined." << std::endl;
    } else {
      std::cerr << "RobotStateCache: Update thread not joinable." << std::endl;
    }
  }
}

void RobotStateCache::updateLoop()
{
  std::cerr << "RobotStateCache: Update loop entered." << std::endl;
  while(running) {
    // Query the server for all robot state
    auto res = spikeClient.executeQuery<spike::AllRobotStateResponse>(
        spike::CommandId::GET_ALL_ROBOT_STATE);
    if(res) {
      std::lock_guard<std::mutex> lock(mtx);
      cachedState = res.value();
      std::cerr << "Client Recv: RMC=" << cachedState.rightMotorCount << ", LMC=" << cachedState.leftMotorCount
                << ", RMP=" << cachedState.rightMotorPower << ", LMP=" << cachedState.leftMotorPower
                << ", RMS=" << cachedState.rightMotorSpeed << ", LMS=" << cachedState.leftMotorSpeed
                << ", Size=" << sizeof(cachedState) << std::endl;
    } else {
      // Handle error or disconnection
      std::cerr << "Error: Failed to get all robot state from server." << std::endl;
    }

    // Sleep for a short period before the next update
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Update every 50ms
  }
  std::cerr << "RobotStateCache: Update loop exited." << std::endl;
}

spike::AllRobotStateResponse RobotStateCache::getCachedState()
{
  std::lock_guard<std::mutex> lock(mtx);
  return cachedState;
}