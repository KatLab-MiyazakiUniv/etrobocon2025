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
        break;
      }
      case spike::CommandId::MOTOR_SET_SPEED: {
        spike::MotorSetSpeedRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        motorHandler.handleSetSpeed(request);
        break;
      }
      case spike::CommandId::MOTOR_STOP_WHEELS: {
        motorHandler.handleStopWheels();
        break;
      }
      case spike::CommandId::MOTOR_BRAKE_WHEELS: {
        motorHandler.handleBrakeWheels();
        break;
      }
      case spike::CommandId::MOTOR_STOP_ARM: {
        motorHandler.handleStopArm();
        break;
      }
      case spike::CommandId::MOTOR_HOLD_ARM: {
        motorHandler.handleHoldArm();
        break;
      }
      case spike::CommandId::MOTOR_GET_COUNT: {
        spike::MotorGetRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        motorHandler.handleGetCount(request);
        break;
      }
      case spike::CommandId::MOTOR_GET_POWER: {
        spike::MotorGetRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        motorHandler.handleGetPower(request);
        break;
      }
      case spike::CommandId::MOTOR_GET_SPEED: {
        spike::MotorGetRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        motorHandler.handleGetSpeed(request);
        break;
      }
      case spike::CommandId::COLOR_SENSOR_GET_REFLECTION: {
        colorSensorHandler.handleGetReflection();
        break;
      }
      case spike::CommandId::COLOR_SENSOR_GET_COLOR_HSV: {
        colorSensorHandler.handleGetColorHsv();
        break;
      }
      case spike::CommandId::CLOCK_SLEEP: {
        spike::ClockSleepRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        clockHandler.handleSleep(request);
        break;
      }
      case spike::CommandId::CLOCK_NOW: {
        clockHandler.handleNow();
        break;
      }
      case spike::CommandId::BUTTON_IS_PRESSED: {
        spike::ButtonIsPressedRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        buttonHandler.handleIsPressed(request);
        break;
      }
      case spike::CommandId::FORCE_SENSOR_IS_PRESSED: {
        spike::ForceSensorIsPressedRequest request;
        if(!receive(client, reinterpret_cast<char*>(&request), sizeof(request))) return;
        forceSensorHandler.handleIsPressed(request);
        break;
      }
      case spike::CommandId::FORCE_SENSOR_GET_FORCE: {
        forceSensorHandler.handleGetForce();
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
      std::cout << "Received CommandId: " << static_cast<uint16_t>(commandId) << std::endl;
      server.handle_command(commandId, client);
    }

    std::cout << "Client disconnected" << std::endl;
    delete client;
  }
}
