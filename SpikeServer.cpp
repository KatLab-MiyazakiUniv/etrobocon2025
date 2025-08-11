#include "SpikeServer.h"
#include "Motor.h"
#include "ColorSensor.h"
#include "Clock.h"
#include "Button.h"
#include "ForceSensor.h"
#include "Display.h"
#include "modules/utils/Socket.h"
#include "modules/common/StringOperator.h"
#include <iostream>
#include <string>
#include <vector>

#define PRESS_POWER 0.5f

void SpikeServer::handle_command(const std::string& cmd, Socket* client)
{
  std::vector<std::string> tokens = StringOperator::split(cmd, ',');
  if(tokens.empty()) {
    client->send("Error: Empty command");
    return;
  }

  static spikeapi::Motor rightWheel(EPort::PORT_A);
  static spikeapi::Motor leftWheel(EPort::PORT_B, spikeapi::Motor::EDirection::COUNTERCLOCKWISE);
  static spikeapi::Motor armMotor(EPort::PORT_C, spikeapi::Motor::EDirection::COUNTERCLOCKWISE);
  static spikeapi::ColorSensor colorSensor(EPort::PORT_E);
  static spikeapi::Clock clock;
  static spikeapi::Button button;
  static spikeapi::ForceSensor forceSensor(EPort::PORT_D);
  static spikeapi::Display display;

  std::string response = "OK";
  std::string class_name = tokens[0];
  std::string method_name = tokens[1];

  if(class_name == "motor") {
    if(method_name == "set_power") {
      std::string target = tokens[2];
      int power = std::stoi(tokens[3]);
      if(target == "right")
        rightWheel.setPower(power);
      else if(target == "left")
        leftWheel.setPower(power);
      else if(target == "arm")
        armMotor.setPower(power);
    } else if(method_name == "set_speed") {
      std::string target = tokens[2];
      double speed = std::stod(tokens[3]);
      int angleSpeed = static_cast<int>(speed / (32.0 * 3.141592 / 360.0) * (180.0 / 3.141592));
      if(target == "right")
        rightWheel.setSpeed(angleSpeed);
      else if(target == "left")
        leftWheel.setSpeed(angleSpeed);
    } else if(method_name == "stop_wheels") {
      rightWheel.stop();
      leftWheel.stop();
    } else if(method_name == "brake_wheels") {
      rightWheel.brake();
      leftWheel.brake();
    } else if(method_name == "stop_arm") {
      armMotor.stop();
    } else if(method_name == "hold_arm") {
      armMotor.hold();
    } else if(method_name == "get_count") {
      std::string target = tokens[2];
      if(target == "right")
        response = std::to_string(rightWheel.getCount());
      else if(target == "left")
        response = std::to_string(leftWheel.getCount());
      else if(target == "arm")
        response = std::to_string(armMotor.getCount());
    } else if(method_name == "get_power") {
      std::string target = tokens[2];
      if(target == "right")
        response = std::to_string(rightWheel.getPower());
      else if(target == "left")
        response = std::to_string(leftWheel.getPower());
      else if(target == "arm")
        response = std::to_string(armMotor.getPower());
    } else if(method_name == "get_speed") {
      std::string target = tokens[2];
      double linearSpeed = 0.0;
      if(target == "right")
        linearSpeed = rightWheel.getSpeed() * (3.141592 / 180.0) * (32.0 * 3.141592 / 360.0);
      else if(target == "left")
        linearSpeed = leftWheel.getSpeed() * (3.141592 / 180.0) * (32.0 * 3.141592 / 360.0);
      response = std::to_string(linearSpeed);
    } else {
      response = "Error: Unknown motor method";
    }
  } else if(class_name == "colorsensor") {
    if(method_name == "get_reflection") {
      response = std::to_string(colorSensor.getReflection());
    } else if(method_name == "get_color_hsv") {
      spikeapi::ColorSensor::HSV hsv;
      colorSensor.getColor(hsv);
      response = std::to_string(hsv.h) + "," + std::to_string(hsv.s) + "," + std::to_string(hsv.v);
    } else {
      response = "Error: Unknown colorsensor method";
    }
  } else if(class_name == "clock") {
    if(method_name == "sleep") {
      clock.sleep(std::stoi(tokens[2]));
    } else if(method_name == "now") {
      response = std::to_string(clock.now());
    }
  } else if(class_name == "button") {
    if(method_name == "is_right_pressed") {
      response = button.isRightPressed() ? "true" : "false";
    } else if(method_name == "is_left_pressed") {
      response = button.isLeftPressed() ? "true" : "false";
    } else if(method_name == "is_center_pressed") {
      response = button.isCenterPressed() ? "true" : "false";
    }
  } else if(class_name == "forcesensor") {
    if(method_name == "is_pressed") {
      response = forceSensor.isPressed(PRESS_POWER) ? "true" : "false";
    } else if(method_name == "get_force") {
      response = std::to_string(forceSensor.getForce());
    }
  } else if(class_name == "display") {
    if(method_name == "show_char") {
      display.showChar(tokens[2][0]);
    }
  } else {
    response = "Error: Unknown class";
  }

  client->send(response);
}

void SpikeServer::start()
{
  Socket server;
  if(!server.create() || !server.bind(8888) || !server.listen()) {
    std::cerr << "Failed to start server" << std::endl;
    return;
  }

  std::cout << "Server started on port 8888" << std::endl;

  while(true) {
    Socket* client = server.accept();
    if(!client) {
      std::cerr << "Failed to accept client" << std::endl;
      continue;
    }

    std::cout << "Client connected" << std::endl;

    std::string command;
    while(client->receive(command) > 0) {
      std::cout << "Received: " << command << std::endl;
      SpikeServer::handle_command(command, client);
    }

    std::cout << "Client disconnected" << std::endl;
    delete client;
  }
}