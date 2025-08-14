/**
 * @file   SpikeServer.cpp
 * @brief  SPIKEデバイスとの通信を管理するサーバークラスの実装
 * @author takuchi17
 */

#include "SpikeServer.h"
#include "Socket.h"
#include "StringOperator.h"
#include "ApiHandler.h"
#include "MotorApiHandler.h"
#include "ColorSensorApiHandler.h"
#include "ClockApiHandler.h"
#include "ButtonApiHandler.h"
#include "ForceSensorApiHandler.h"
#include "DisplayApiHandler.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>    // For memcpy
#include <stdexcept>  // For std::runtime_error

// SpikeServer constructor
SpikeServer::SpikeServer(Socket* client)
  : motorHandler(client),
    colorSensorHandler(client),
    clockHandler(client),
    buttonHandler(client),
    forceSensorHandler(client),
    displayHandler(client)
{
}

// Helper to receive data (non-static now)
bool SpikeServer::receive(Socket* client, char* buffer, size_t size)
{
  bool success = client->receiveData(buffer, size);
  if(!success) {
    std::cerr << "Error: Expected " << size << " bytes, but received " << std::endl;
  }
  return success;
}

// Helper to send data (non-static now)
bool SpikeServer::send(Socket* client, const char* buffer, size_t size)
{
  bool success = client->sendData(buffer, size);
  if(!success) {
    std::cerr << "Error: Expected to send " << size << " bytes, but sent " << std::endl;
  }
  return success;
}

void SpikeServer::handle_command(spike::CommandId commandId, Socket* client)
{
  spike::Response response_header;
  response_header.value = true;  // Assume success unless an error occurs

  try {
    switch(commandId) {
      case spike::CommandId::MOTOR_SET_POWER: {
        spike::MotorSetPowerRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        motorHandler.handleSetPower(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
      }
      case spike::CommandId::MOTOR_SET_SPEED: {
        spike::MotorSetSpeedRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        motorHandler.handleSetSpeed(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
      }
      case spike::CommandId::MOTOR_STOP_WHEELS: {
        motorHandler.handleStopWheels();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
      }
      case spike::CommandId::MOTOR_BRAKE_WHEELS: {
        motorHandler.handleBrakeWheels();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
      }
      case spike::CommandId::MOTOR_STOP_ARM: {
        motorHandler.handleStopArm();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
      }
      case spike::CommandId::MOTOR_HOLD_ARM: {
        motorHandler.handleHoldArm();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
      }
      case spike::CommandId::MOTOR_GET_COUNT: {
        spike::MotorGetRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        spike::Int32Response res = motorHandler.handleGetCount(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::MOTOR_GET_POWER: {
        spike::MotorGetRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        spike::Int32Response res = motorHandler.handleGetPower(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::MOTOR_GET_SPEED: {
        spike::MotorGetRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        spike::Int32Response res = motorHandler.handleGetSpeed(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::COLOR_SENSOR_GET_REFLECTION: {
        spike::Int32Response res = colorSensorHandler.handleGetReflection();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::COLOR_SENSOR_GET_COLOR_HSV: {
        spike::HsvResponse res = colorSensorHandler.handleGetColorHsv();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::CLOCK_SLEEP: {
        spike::ClockSleepRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        clockHandler.handleSleep(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
      }
      case spike::CommandId::CLOCK_NOW: {
        spike::UInt64Response res = clockHandler.handleNow();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::BUTTON_IS_PRESSED: {
        spike::ButtonIsPressedRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        spike::BoolResponse res = buttonHandler.handleIsPressed(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::FORCE_SENSOR_IS_PRESSED: {
        spike::ForceSensorIsPressedRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        spike::BoolResponse res = forceSensorHandler.handleIsPressed(request);
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::FORCE_SENSOR_GET_FORCE: {
        spike::FloatResponse res = forceSensorHandler.handleGetForce();
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        break;
      }
      case spike::CommandId::DISPLAY_SHOW_CHAR: {
        spike::DisplayShowCharRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        displayHandler.handleShowChar(request);
        break;
      }
      case spike::CommandId::DISPLAY_SHOW_NUMBER: {
        spike::DisplayShowNumberRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        displayHandler.handleShowNumber(request);
        break;
      }
      case spike::CommandId::DISPLAY_SCROLL_TEXT: {
        spike::DisplayScrollTextRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        displayHandler.handleScrollText(request);
        break;
      }
      case spike::CommandId::GET_ALL_ROBOT_STATE: {
        spike::AllRobotStateResponse res;
        res.rightMotorCount = motorHandler.handleGetCount({ spike::MotorTarget::RIGHT }).value;
        res.leftMotorCount = motorHandler.handleGetCount({ spike::MotorTarget::LEFT }).value;
        res.rightMotorPower = motorHandler.handleGetPower({ spike::MotorTarget::RIGHT }).value;
        res.leftMotorPower = motorHandler.handleGetPower({ spike::MotorTarget::LEFT }).value;
        res.rightMotorSpeed = motorHandler.handleGetSpeed({ spike::MotorTarget::RIGHT }).value;
        res.leftMotorSpeed = motorHandler.handleGetSpeed({ spike::MotorTarget::LEFT }).value;
        res.reflection = colorSensorHandler.handleGetReflection().value;
        spike::HsvResponse hsv = colorSensorHandler.handleGetColorHsv();
        res.hsv_h = hsv.h;
        res.hsv_s = hsv.s;
        res.hsv_v = hsv.v;
        res.forceSensorPressed = forceSensorHandler.handleIsPressed({ 0.5f }).value;
        res.forceSensorForce = forceSensorHandler.handleGetForce().value;
        res.buttonPressedRight
            = buttonHandler.handleIsPressed({ spike::ButtonTarget::RIGHT }).value;
        res.buttonPressedLeft = buttonHandler.handleIsPressed({ spike::ButtonTarget::LEFT }).value;
        res.buttonPressedCenter
            = buttonHandler.handleIsPressed({ spike::ButtonTarget::CENTER }).value;
        res.clockNow = clockHandler.handleNow().value;

        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        send(client, reinterpret_cast<char*>(&res), sizeof(res));
        std::cerr << "Server: Finished GET_ALL_ROBOT_STATE" << std::endl;
        break;
      }
      default:
        std::cerr << "Error: Unknown CommandId received: " << static_cast<uint16_t>(commandId)
                  << std::endl;
        response_header.value = false;
        send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
        break;
    }
  } catch(const std::exception& e) {
    std::cerr << "Error handling command: " << e.what() << std::endl;
    response_header.value = false;
    send(client, reinterpret_cast<char*>(&response_header), sizeof(response_header));
  }
}

void SpikeServer::start()
{
  Socket server_socket;  // Use a local variable for the server socket
  if(!server_socket.create() || !server_socket.bind(8888) || !server_socket.listen()) {
    std::cerr << "Failed to start server" << std::endl;
    return;
  }

  std::cout << "Server started on port 8888" << std::endl;

  while(true) {
    Socket* client = server_socket.accept();  // Accept client connection
    if(!client) {
      std::cerr << "Failed to accept client" << std::endl;
      continue;
    }

    std::cout << "Client connected" << std::endl;

    SpikeServer server(client);
    while(true) {
      spike::CommandId commandId;
      if(!server.receive(client, reinterpret_cast<char*>(&commandId), sizeof(commandId))) {
        // Client disconnected or error
        break;
      }
      server.handle_command(commandId, client);
    }

    std::cout << "Client disconnected" << std::endl;
    delete client;
  }
}
