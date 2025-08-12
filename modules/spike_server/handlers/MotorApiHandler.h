#ifndef MOTOR_API_HANDLER_H
#define MOTOR_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "Motor.h"

class MotorApiHandler : public ApiHandler {  // Inherit from ApiHandler
 public:
  explicit MotorApiHandler(Socket* client);  // Use explicit for single-argument constructor

  void handleSetPower(const spike::MotorSetPowerRequest& request);
  void handleSetSpeed(const spike::MotorSetSpeedRequest& request);
  void handleStopWheels();
  void handleBrakeWheels();
  void handleStopArm();
  void handleHoldArm();
  void handleGetCount(const spike::MotorGetRequest& request);
  void handleGetPower(const spike::MotorGetRequest& request);
  void handleGetSpeed(const spike::MotorGetRequest& request);

 private:
  // client_ and send() are now inherited from ApiHandler
  spikeapi::Motor rightWheel_;
  spikeapi::Motor leftWheel_;
  spikeapi::Motor armMotor_;
};

#endif  // MOTOR_API_HANDLER_H
