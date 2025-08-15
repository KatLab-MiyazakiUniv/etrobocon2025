/**
 * @file   MotorApiHandler.cpp
 * @brief  モーターAPIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "MotorApiHandler.h"
#include "Motor.h"
#include <iostream>

MotorApiHandler::MotorApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    rightWheel(EPort::PORT_A),
    leftWheel(EPort::PORT_B, spikeapi::Motor::EDirection::COUNTERCLOCKWISE),
    armMotor(EPort::PORT_C, spikeapi::Motor::EDirection::COUNTERCLOCKWISE)
{
}

// send() is now inherited from ApiHandler

void MotorApiHandler::handleSetPower(const spike::MotorSetPowerRequest& request)
{
  spike::Response response;
  response.value = false;
  if(request.target == spike::MotorTarget::RIGHT) {
    rightWheel.setPower(request.power);
  } else if(request.target == spike::MotorTarget::LEFT) {
    leftWheel.setPower(request.power);
  } else if(request.target == spike::MotorTarget::ARM) {
    armMotor.setPower(request.power);
  } else {
    response.value = false;
    std::cerr << "Error: Unknown MotorTarget for setPower" << std::endl;
  }
}

void MotorApiHandler::handleSetSpeed(const spike::MotorSetSpeedRequest& request)
{
  spike::Response response;
  response.value = true;
  int angleSpeed = static_cast<int>(request.speed);
  if(request.target == spike::MotorTarget::RIGHT) {
    rightWheel.setSpeed(angleSpeed);
  } else if(request.target == spike::MotorTarget::LEFT) {
    leftWheel.setSpeed(angleSpeed);
  } else {
    response.value = false;
    std::cerr << "Error: Unknown MotorTarget for setSpeed" << std::endl;
  }
}

void MotorApiHandler::handleStopWheels()
{
  spike::Response response;
  response.value = true;
  rightWheel.stop();
  leftWheel.stop();
}

void MotorApiHandler::handleBrakeWheels()
{
  spike::Response response;
  response.value = true;
  rightWheel.brake();
  leftWheel.brake();
}

void MotorApiHandler::handleStopArm()
{
  spike::Response response;
  response.value = true;
  armMotor.stop();
}

void MotorApiHandler::handleHoldArm()
{
  spike::Response response;
  response.value = true;
  armMotor.hold();
}

spike::Int32Response MotorApiHandler::handleGetCount(const spike::MotorGetRequest& request)
{
  spike::Int32Response response;
  response.value = -1;
  if(request.target == spike::MotorTarget::RIGHT) {
    response.value = rightWheel.getCount();
  } else if(request.target == spike::MotorTarget::LEFT) {
    response.value = leftWheel.getCount();
  } else if(request.target == spike::MotorTarget::ARM) {
    response.value = armMotor.getCount();
  } else {
    response.value = -1;
    std::cerr << "Error: Unknown MotorTarget for getCount" << std::endl;
  }
  return response;
}

spike::Int32Response MotorApiHandler::handleGetPower(const spike::MotorGetRequest& request)
{
  spike::Int32Response response;
  response.value = -1;
  if(request.target == spike::MotorTarget::RIGHT) {
    response.value = rightWheel.getPower();
  } else if(request.target == spike::MotorTarget::LEFT) {
    response.value = leftWheel.getPower();
  } else if(request.target == spike::MotorTarget::ARM) {
    response.value = armMotor.getPower();
  } else {
    response.value = -1;
    std::cerr << "Error: Unknown MotorTarget for getPower" << std::endl;
  }
  return response;
}

spike::Int32Response MotorApiHandler::handleGetSpeed(const spike::MotorGetRequest& request)
{
  spike::Int32Response response;
  response.value = 0;
  if(request.target == spike::MotorTarget::RIGHT) {
    response.value = rightWheel.getSpeed();
  } else if(request.target == spike::MotorTarget::LEFT) {
    response.value = leftWheel.getSpeed();
  } else {
    std::cerr << "Error: Unknown MotorTarget for getSpeed" << std::endl;
  }
  return response;
}
