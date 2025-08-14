#include "SpikeClient.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

// Helper to create a payload from a value
template <typename T>
std::vector<char> create_payload(const T& value)
{
  std::vector<char> payload(sizeof(T));
  std::memcpy(payload.data(), &value, sizeof(T));
  return payload;
}

SpikeClient::SpikeClient()
{
  // Initialize motor states
  motor_states_[spike::MotorTarget::LEFT] = MockMotorState();
  motor_states_[spike::MotorTarget::RIGHT] = MockMotorState();
  motor_states_[spike::MotorTarget::ARM] = MockMotorState();

  // === Button Handlers ===
  setCommandHandler(spike::CommandId::BUTTON_IS_PRESSED, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::ButtonTarget)) return MockResponse{ false, {} };
    spike::ButtonTarget target;
    std::memcpy(&target, req.data(), sizeof(spike::ButtonTarget));

    if(this->button_press_queues_.count(target) && !this->button_press_queues_[target].empty()) {
      bool isPressed = this->button_press_queues_[target].front();
      this->button_press_queues_[target].pop();
      return MockResponse{ true, create_payload(isPressed) };
    }

    bool isPressed = this->button_states_.count(target) ? this->button_states_[target] : false;
    return MockResponse{ true, create_payload(isPressed) };
  });

  // === Clock Handlers ===
  setCommandHandler(spike::CommandId::CLOCK_SLEEP, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::ClockSleepRequest)) return MockResponse{ false, {} };
    spike::ClockSleepRequest sleep_req;
    std::memcpy(&sleep_req, req.data(), sizeof(spike::ClockSleepRequest));
    this->time_ += sleep_req.microseconds;
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::CLOCK_NOW, [this](const std::vector<char>& req) {
    return MockResponse{ true, create_payload(this->time_) };
  });

  // === Color Sensor Handlers ===
  setCommandHandler(spike::CommandId::COLOR_SENSOR_GET_REFLECTION,
                    [this](const std::vector<char>& req) {
                      if(!reflection_queue_.empty()) {
                        int32_t reflection = reflection_queue_.front();
                        reflection_queue_.pop();
                        return MockResponse{ true, create_payload(reflection) };
                      }
                      return MockResponse{ true, create_payload(this->reflection_) };
                    });

  setCommandHandler(spike::CommandId::COLOR_SENSOR_GET_COLOR_HSV,
                    [this](const std::vector<char>& req) {
                      if(!hsv_queue_.empty()) {
                        spike::HsvResponse hsv = hsv_queue_.front();
                        hsv_queue_.pop();
                        return MockResponse{ true, create_payload(hsv) };
                      }
                      return MockResponse{ true, create_payload(this->hsv_) };
                    });

  // === Display Handlers (no return values, just acknowledge) ===
  setCommandHandler(spike::CommandId::DISPLAY_SHOW_CHAR,
                    [](const std::vector<char>& req) { return MockResponse{ true, {} }; });
  setCommandHandler(spike::CommandId::DISPLAY_SHOW_NUMBER,
                    [](const std::vector<char>& req) { return MockResponse{ true, {} }; });
  setCommandHandler(spike::CommandId::DISPLAY_SCROLL_TEXT,
                    [](const std::vector<char>& req) { return MockResponse{ true, {} }; });

  // === Force Sensor Handlers ===
  setCommandHandler(spike::CommandId::FORCE_SENSOR_IS_PRESSED,
                    [this](const std::vector<char>& req) {
                      return MockResponse{ true, create_payload(this->force_sensor_pressed_) };
                    });
  setCommandHandler(spike::CommandId::FORCE_SENSOR_GET_FORCE, [this](const std::vector<char>& req) {
    return MockResponse{ true, create_payload(this->force_) };
  });

  // === Motor Handlers ===
  setCommandHandler(spike::CommandId::MOTOR_SET_POWER, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::MotorSetPowerRequest)) return MockResponse{ false, {} };
    spike::MotorSetPowerRequest p_req;
    std::memcpy(&p_req, req.data(), sizeof(spike::MotorSetPowerRequest));
    auto& motor = this->motor_states_[p_req.target];
    motor.power = p_req.power;
    motor.speed = p_req.power * 5;  // Example conversion
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::MOTOR_SET_SPEED, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::MotorSetSpeedRequest)) return MockResponse{ false, {} };
    spike::MotorSetSpeedRequest s_req;
    std::memcpy(&s_req, req.data(), sizeof(spike::MotorSetSpeedRequest));
    auto& motor = this->motor_states_[s_req.target];
    motor.speed = s_req.speed;
    motor.power = s_req.speed / 5;  // Example conversion
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::MOTOR_STOP_WHEELS, [this](const std::vector<char>& req) {
    this->motor_states_[spike::MotorTarget::LEFT].power = 0;
    this->motor_states_[spike::MotorTarget::LEFT].speed = 0;
    this->motor_states_[spike::MotorTarget::RIGHT].power = 0;
    this->motor_states_[spike::MotorTarget::RIGHT].speed = 0;
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::MOTOR_BRAKE_WHEELS, [this](const std::vector<char>& req) {
    this->motor_states_[spike::MotorTarget::LEFT].power = 0;
    this->motor_states_[spike::MotorTarget::LEFT].speed = 0;
    this->motor_states_[spike::MotorTarget::RIGHT].power = 0;
    this->motor_states_[spike::MotorTarget::RIGHT].speed = 0;
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::MOTOR_STOP_ARM, [this](const std::vector<char>& req) {
    this->motor_states_[spike::MotorTarget::ARM].power = 0;
    this->motor_states_[spike::MotorTarget::ARM].speed = 0;
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::MOTOR_HOLD_ARM, [this](const std::vector<char>& req) {
    this->motor_states_[spike::MotorTarget::ARM].speed = 0;  // Holding means stopping speed
    return MockResponse{ true, {} };
  });

  // === Motor Handlers ===
  setCommandHandler(spike::CommandId::MOTOR_SET_POWER, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::MotorSetPowerRequest)) return MockResponse{ false, {} };
    spike::MotorSetPowerRequest p_req;
    std::memcpy(&p_req, req.data(), sizeof(spike::MotorSetPowerRequest));
    auto& motor = this->motor_states_[p_req.target];
    motor.power = p_req.power;
    motor.speed = p_req.power * 5;
    motor.count += static_cast<int32_t>(motor.speed * 0.02);
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::MOTOR_SET_SPEED, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::MotorSetSpeedRequest)) return MockResponse{ false, {} };
    spike::MotorSetSpeedRequest s_req;
    std::memcpy(&s_req, req.data(), sizeof(spike::MotorSetSpeedRequest));
    auto& motor = this->motor_states_[s_req.target];
    motor.speed = s_req.speed;
    motor.power = s_req.speed / 5;
    motor.count += static_cast<int32_t>(motor.speed * 0.02);
    return MockResponse{ true, {} };
  });

  setCommandHandler(spike::CommandId::MOTOR_GET_COUNT, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::MotorTarget)) return MockResponse{ false, {} };
    spike::MotorTarget target;
    std::memcpy(&target, req.data(), sizeof(spike::MotorTarget));
    auto& motor = this->motor_states_[target];
    motor.count += static_cast<int32_t>(motor.speed * 0.02);
    return MockResponse{ true, create_payload(motor.count) };
  });

  setCommandHandler(spike::CommandId::MOTOR_GET_POWER, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::MotorTarget)) return MockResponse{ false, {} };
    spike::MotorTarget target;
    std::memcpy(&target, req.data(), sizeof(spike::MotorTarget));
    return MockResponse{ true, create_payload(this->motor_states_[target].power) };
  });

  setCommandHandler(spike::CommandId::MOTOR_GET_SPEED, [this](const std::vector<char>& req) {
    if(req.size() < sizeof(spike::MotorTarget)) return MockResponse{ false, {} };
    spike::MotorTarget target;
    std::memcpy(&target, req.data(), sizeof(spike::MotorTarget));
    return MockResponse{ true, create_payload(this->motor_states_[target].speed) };
  });
}

bool SpikeClient::connect(const std::string& host, int port)
{
  return true;  // Mock connection always succeeds
}

void SpikeClient::disconnect()
{
  // Mock disconnection, do nothing
}

void SpikeClient::setCommandHandler(spike::CommandId id, CommandHandler handler)
{
  commandHandlers_[id] = handler;
}

MockResponse SpikeClient::handleCommand(spike::CommandId id,
                                        const std::vector<char>& request_payload)
{
  if(commandHandlers_.count(id)) {
    return commandHandlers_[id](request_payload);
  } else {
    std::cerr << "Error: No handler registered for CommandId: " << static_cast<uint16_t>(id)
              << std::endl;
    return { false, {} };
  }
}

// Mock control method implementations
void SpikeClient::setButtonPressed(spike::ButtonTarget target, bool pressed)
{
  button_states_[target] = pressed;
}

void SpikeClient::queueButtonPressed(spike::ButtonTarget target, const std::vector<bool>& presses)
{
  for(const auto& p : presses) {
    button_press_queues_[target].push(p);
  }
}

void SpikeClient::setReflection(int32_t reflection)
{
  reflection_ = reflection;
  // Clear any queued values to avoid ambiguity
  std::queue<int32_t> empty;
  reflection_queue_.swap(empty);
}

void SpikeClient::queueReflection(const std::vector<int32_t>& reflections)
{
  for(const auto& r : reflections) {
    reflection_queue_.push(r);
  }
}

void SpikeClient::setHsv(const spike::HsvResponse& hsv)
{
  hsv_ = hsv;
  // Clear any queued values to avoid ambiguity
  std::queue<spike::HsvResponse> empty;
  hsv_queue_.swap(empty);
}

void SpikeClient::queueHsv(const std::vector<spike::HsvResponse>& hsvs)
{
  for(const auto& hsv : hsvs) {
    hsv_queue_.push(hsv);
  }
}

void SpikeClient::setForceSensorPressed(bool pressed)
{
  force_sensor_pressed_ = pressed;
}

void SpikeClient::setForce(float force)
{
  force_ = force;
}

void SpikeClient::setMotorCount(spike::MotorTarget target, int32_t count)
{
  motor_states_[target].count = count;
}

void SpikeClient::setMotorPower(spike::MotorTarget target, int32_t power)
{
  motor_states_[target].power = power;
}

void SpikeClient::setMotorSpeed(spike::MotorTarget target, int32_t speed)
{
  motor_states_[target].speed = speed;
}

int32_t SpikeClient::getMotorCount(spike::MotorTarget target)
{
  return motor_states_[target].count;
}

int32_t SpikeClient::getMotorPower(spike::MotorTarget target)
{
  return motor_states_[target].power;
}

int32_t SpikeClient::getMotorSpeed(spike::MotorTarget target)
{
  return motor_states_[target].speed;
}

uint64_t SpikeClient::getMockTime()
{
  return time_;
}