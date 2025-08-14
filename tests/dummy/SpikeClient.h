#ifndef DUMMY_SPIKE_CLIENT_H
#define DUMMY_SPIKE_CLIENT_H

#include "SpikeCommand.h"
#include <string>
#include <vector>
#include <optional>
#include <map>
#include <functional>
#include <cstring>  // For memcpy
#include <queue>

// Define a type for the response data and a map for handlers
struct MockResponse {
  bool success;
  std::vector<char> payload;
};

// Using std::function for more flexible handlers
using CommandHandler = std::function<MockResponse(const std::vector<char>& request_payload)>;

class SpikeClient {
 public:
  SpikeClient();
  bool connect(const std::string& host, int port);
  void disconnect();

  // Method to set up expected responses for commands
  void setCommandHandler(spike::CommandId id, CommandHandler handler);

  // Mock control methods to manipulate the dummy's state from tests
  void setButtonPressed(spike::ButtonTarget target, bool pressed);
  void queueButtonPressed(spike::ButtonTarget target, const std::vector<bool>& presses);
  void setReflection(int32_t reflection);
  void queueReflection(const std::vector<int32_t>& reflections);
  void setHsv(const spike::HsvResponse& hsv);
  void queueHsv(const std::vector<spike::HsvResponse>& hsvs);
  void setForceSensorPressed(bool pressed);
  void setForce(float force);
  void setMotorCount(spike::MotorTarget target, int32_t count);
  void setMotorPower(spike::MotorTarget target, int32_t power);
  void setMotorSpeed(spike::MotorTarget target, int32_t speed);
  int32_t getMotorCount(spike::MotorTarget target);
  int32_t getMotorPower(spike::MotorTarget target);
  int32_t getMotorSpeed(spike::MotorTarget target);
  uint64_t getMockTime();

  inline bool executeCommand(spike::CommandId id)
  {
    return executeCommand(id, std::vector<char>{});  // No request payload
  }

  template <typename TRequest>
  bool executeCommand(spike::CommandId id, const TRequest& request)
  {
    std::vector<char> request_payload(sizeof(TRequest));
    std::memcpy(request_payload.data(), &request, sizeof(TRequest));
    return handleCommand(id, request_payload).success;
  }

  template <typename TResponse, typename TRequest>
  std::optional<TResponse> executeQuery(spike::CommandId id, const TRequest& request)
  {
    std::vector<char> request_payload(sizeof(TRequest));
    std::memcpy(request_payload.data(), &request, sizeof(TRequest));
    MockResponse mock_res = handleCommand(id, request_payload);
    if(!mock_res.success || mock_res.payload.empty()) return std::nullopt;
    TResponse response_payload;
    std::memcpy(&response_payload, mock_res.payload.data(),
                std::min(mock_res.payload.size(), sizeof(TResponse)));
    return response_payload;
  }

  template <typename TResponse>
  std::optional<TResponse> executeQuery(spike::CommandId id)
  {
    MockResponse mock_res = handleCommand(id, std::vector<char>{});  // No request payload
    if(!mock_res.success || mock_res.payload.empty()) return std::nullopt;
    TResponse response_payload;
    std::memcpy(&response_payload, mock_res.payload.data(),
                std::min(mock_res.payload.size(), sizeof(TResponse)));
    return response_payload;
  }

 private:
  // Internal handler that dispatches to registered handlers
  MockResponse handleCommand(spike::CommandId id, const std::vector<char>& request_payload);

  std::map<spike::CommandId, CommandHandler> commandHandlers_;

  // Mocked device states
  std::map<spike::ButtonTarget, bool> button_states_;
  std::map<spike::ButtonTarget, std::queue<bool>> button_press_queues_;
  std::map<spike::ButtonTarget, int> button_press_counts_;
  uint64_t time_ = 0;
  int32_t reflection_ = 20;  // Default to black
  std::queue<int32_t> reflection_queue_;
  spike::HsvResponse hsv_{ 0, 0, 0 }; // Default to black
  std::queue<spike::HsvResponse> hsv_queue_;
  int hsv_get_count_ = 0;
  bool force_sensor_pressed_ = false;
  int force_sensor_press_count_ = 0;
  float force_ = 0.0f;

  struct MockMotorState {
    int32_t count = 0;
    int32_t power = 0;
    int32_t speed = 0;
  };
  std::map<spike::MotorTarget, MockMotorState> motor_states_;
};

#endif  // DUMMY_SPIKE_CLIENT_H