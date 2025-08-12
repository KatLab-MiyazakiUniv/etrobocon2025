#include "MotorApiHandler.h"
#include "Motor.h"
#include <iostream>

MotorApiHandler::MotorApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    rightWheel_(EPort::PORT_A),
    leftWheel_(EPort::PORT_B, spikeapi::Motor::EDirection::COUNTERCLOCKWISE),
    armMotor_(EPort::PORT_C, spikeapi::Motor::EDirection::COUNTERCLOCKWISE)
{
}

// send() is now inherited from ApiHandler

void MotorApiHandler::handleSetPower(const spike::MotorSetPowerRequest& request)
{
  spike::Response response;
  response.value = false;
  if(request.target == spike::MotorTarget::RIGHT) {
    rightWheel_.setPower(request.power);
  } else if(request.target == spike::MotorTarget::LEFT) {
    leftWheel_.setPower(request.power);
  } else if(request.target == spike::MotorTarget::ARM) {
    armMotor_.setPower(request.power);
  } else {
    response.value = false;
    std::cerr << "Error: Unknown MotorTarget for setPower" << std::endl;
  }
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleSetSpeed(const spike::MotorSetSpeedRequest& request)
{
  spike::Response response;
  response.value = true;
  int angleSpeed = static_cast<int>(request.speed / (32.0 * 3.141592 / 360.0) * (180.0 / 3.141592));
  if(request.target == spike::MotorTarget::RIGHT) {
    rightWheel_.setSpeed(angleSpeed);
  } else if(request.target == spike::MotorTarget::LEFT) {
    leftWheel_.setSpeed(angleSpeed);
  } else {
    response.value = false;
    std::cerr << "Error: Unknown MotorTarget for setSpeed" << std::endl;
  }
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleStopWheels()
{
  spike::Response response;
  response.value = true;
  rightWheel_.stop();
  leftWheel_.stop();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleBrakeWheels()
{
  spike::Response response;
  response.value = true;
  rightWheel_.brake();
  leftWheel_.brake();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleStopArm()
{
  spike::Response response;
  response.value = true;
  armMotor_.stop();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleHoldArm()
{
  spike::Response response;
  response.value = true;
  armMotor_.hold();
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleGetCount(const spike::MotorGetRequest& request)
{
  spike::Int32Response response;
  response.value = -1;
  if(request.target == spike::MotorTarget::RIGHT) {
    response.value = rightWheel_.getCount();
  } else if(request.target == spike::MotorTarget::LEFT) {
    response.value = leftWheel_.getCount();
  } else if(request.target == spike::MotorTarget::ARM) {
    response.value = armMotor_.getCount();
  } else {
    response.value = -1;
    std::cerr << "Error: Unknown MotorTarget for getCount" << std::endl;
  }
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleGetPower(const spike::MotorGetRequest& request)
{
  spike::Int32Response response;
  response.value = -1;
  if(request.target == spike::MotorTarget::RIGHT) {
    response.value = rightWheel_.getPower();
  } else if(request.target == spike::MotorTarget::LEFT) {
    response.value = leftWheel_.getPower();
  } else if(request.target == spike::MotorTarget::ARM) {
    response.value = armMotor_.getPower();
  } else {
    response.value = -1;
    std::cerr << "Error: Unknown MotorTarget for getPower" << std::endl;
  }
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void MotorApiHandler::handleGetSpeed(const spike::MotorGetRequest& request)
{
  spike::DoubleResponse response;
  response.value = true;
  double linearSpeed = 0.0;
  if(request.target == spike::MotorTarget::RIGHT) {
    linearSpeed = rightWheel_.getSpeed() * (3.141592 / 180.0) * (32.0 * 3.141592 / 360.0);
  } else if(request.target == spike::MotorTarget::LEFT) {
    linearSpeed = leftWheel_.getSpeed() * (3.141592 / 180.0) * (32.0 * 3.141592 / 360.0);
  } else {
    response.value = false;
    std::cerr << "Error: Unknown MotorTarget for getSpeed" << std::endl;
  }
  response.value = linearSpeed;
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
