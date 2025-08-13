
#include "SpikeServerMock.h"
#include "Socket.h"
#include "SpikeCommand.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>

// Initialize static member variables
int32_t SpikeServerMock::right_motor_count = 0;
int32_t SpikeServerMock::left_motor_count = 0;
int32_t SpikeServerMock::arm_motor_count = 0;
int32_t SpikeServerMock::right_motor_power = 0;
int32_t SpikeServerMock::left_motor_power = 0;
int32_t SpikeServerMock::arm_motor_power = 0;
double SpikeServerMock::right_motor_speed = 0.0;
double SpikeServerMock::left_motor_speed = 0.0;
int32_t SpikeServerMock::reflection_value = 50;                          // Default middle value
spike::HsvResponse SpikeServerMock::hsv_value = { 180.0f, 0.5f, 0.8f };  // Default cyan-ish
bool SpikeServerMock::button_pressed_state[3] = { false, false, false };
bool SpikeServerMock::force_sensor_pressed_state = false;
double SpikeServerMock::force_sensor_value = 0.0;
char SpikeServerMock::display_char = ' ';
int8_t SpikeServerMock::display_number = 0;
std::string SpikeServerMock::display_scroll_text = "";

// Random number generator for dynamic values
std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> reflection_distribution(0, 100);
std::uniform_real_distribution<float> hsv_h_distribution(0.0f, 360.0f);
std::uniform_real_distribution<float> hsv_sv_distribution(0.0f, 1.0f);
std::uniform_int_distribution<int> bool_distribution(0, 1);
std::uniform_real_distribution<double> force_distribution(0.0, 10.0);

// Helper function to handle receiving data
bool receiveData(Socket* client, void* buffer, size_t size)
{
  if(!client->receiveData(buffer, size)) {
    std::cerr << "MockServer: Failed to receive data or client disconnected." << std::endl;
    return false;
  }
  return true;
}

// Helper function to handle sending data
bool sendData(Socket* client, const void* buffer, size_t size)
{
  if(!client->sendData(buffer, size)) {
    std::cerr << "MockServer: Failed to send data or client disconnected." << std::endl;
    return false;
  }
  return true;
}

void handle_client(Socket* client)
{
  std::cout << "MockServer: Client connected." << std::endl;

  while(true) {
    spike::Request request;
    if(!receiveData(client, &request, sizeof(request))) {
      break;  // Exit loop if client disconnects
    }

    spike::Response response_header{ true };

    std::cout << "MockServer: Received Command ID: " << static_cast<uint16_t>(request.id)
              << std::endl;

    switch(request.id) {
      // Motor Commands
      case spike::CommandId::MOTOR_SET_POWER: {
        spike::MotorSetPowerRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        std::cout << "MockServer: MOTOR_SET_POWER - Target: " << static_cast<int>(req.target)
                  << ", Power: " << req.power << std::endl;
        if(req.target == spike::MotorTarget::RIGHT)
          SpikeServerMock::right_motor_power = req.power;
        else if(req.target == spike::MotorTarget::LEFT)
          SpikeServerMock::left_motor_power = req.power;
        else if(req.target == spike::MotorTarget::ARM)
          SpikeServerMock::arm_motor_power = req.power;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::MOTOR_SET_SPEED: {
        spike::MotorSetSpeedRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        std::cout << "MockServer: MOTOR_SET_SPEED - Target: " << static_cast<int>(req.target)
                  << ", Speed: " << req.speed << std::endl;
        if(req.target == spike::MotorTarget::RIGHT)
          SpikeServerMock::right_motor_speed = req.speed;
        else if(req.target == spike::MotorTarget::LEFT)
          SpikeServerMock::left_motor_speed = req.speed;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::MOTOR_STOP_WHEELS: {
        std::cout << "MockServer: MOTOR_STOP_WHEELS" << std::endl;
        SpikeServerMock::right_motor_power = 0;
        SpikeServerMock::left_motor_power = 0;
        SpikeServerMock::right_motor_speed = 0.0;
        SpikeServerMock::left_motor_speed = 0.0;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::MOTOR_BRAKE_WHEELS: {
        std::cout << "MockServer: MOTOR_BRAKE_WHEELS" << std::endl;
        SpikeServerMock::right_motor_power = 0;
        SpikeServerMock::left_motor_power = 0;
        SpikeServerMock::right_motor_speed = 0.0;
        SpikeServerMock::left_motor_speed = 0.0;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::MOTOR_STOP_ARM: {
        std::cout << "MockServer: MOTOR_STOP_ARM" << std::endl;
        SpikeServerMock::arm_motor_power = 0;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::MOTOR_HOLD_ARM: {
        std::cout << "MockServer: MOTOR_HOLD_ARM" << std::endl;
        // In a mock, holding means maintaining current power/position, which is already the default
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::MOTOR_GET_COUNT: {
        spike::MotorGetRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        spike::Int32Response res;
        if(req.target == spike::MotorTarget::RIGHT)
          res.value = SpikeServerMock::right_motor_count += 1000;
        else if(req.target == spike::MotorTarget::LEFT)
          res.value = SpikeServerMock::left_motor_count += 1000;
        else if(req.target == spike::MotorTarget::ARM)
          res.value = SpikeServerMock::arm_motor_count += 1000;
        else
          res.value = 0;  // Should not happen
        std::cout << "MockServer: MOTOR_GET_COUNT - Target: " << static_cast<int>(req.target)
                  << ", Count: " << res.value << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }
      case spike::CommandId::MOTOR_GET_POWER: {
        spike::MotorGetRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        spike::Int32Response res;
        if(req.target == spike::MotorTarget::RIGHT)
          res.value = SpikeServerMock::right_motor_power;
        else if(req.target == spike::MotorTarget::LEFT)
          res.value = SpikeServerMock::left_motor_power;
        else if(req.target == spike::MotorTarget::ARM)
          res.value = SpikeServerMock::arm_motor_power;
        else
          res.value = 0;  // Should not happen
        std::cout << "MockServer: MOTOR_GET_POWER - Target: " << static_cast<int>(req.target)
                  << ", Power: " << res.value << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }
      case spike::CommandId::MOTOR_GET_SPEED: {
        spike::MotorGetRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        spike::DoubleResponse res;
        if(req.target == spike::MotorTarget::RIGHT)
          res.value = SpikeServerMock::right_motor_speed;
        else if(req.target == spike::MotorTarget::LEFT)
          res.value = SpikeServerMock::left_motor_speed;
        else
          res.value = 0.0;  // Should not happen
        std::cout << "MockServer: MOTOR_GET_SPEED - Target: " << static_cast<int>(req.target)
                  << ", Speed: " << res.value << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }

      // Color Sensor Commands
      case spike::CommandId::COLOR_SENSOR_GET_REFLECTION: {
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        SpikeServerMock::reflection_value
            = reflection_distribution(generator);  // Random reflection
        spike::Int32Response res{ SpikeServerMock::reflection_value };
        std::cout << "MockServer: COLOR_SENSOR_GET_REFLECTION - Value: " << res.value << std::endl;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }
      case spike::CommandId::COLOR_SENSOR_GET_COLOR_HSV: {
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        // Cycle through some colors or random
        SpikeServerMock::hsv_value.h = hsv_h_distribution(generator);
        SpikeServerMock::hsv_value.s = hsv_sv_distribution(generator);
        SpikeServerMock::hsv_value.v = hsv_sv_distribution(generator);
        spike::HsvResponse res = SpikeServerMock::hsv_value;
        std::cout << "MockServer: COLOR_SENSOR_GET_COLOR_HSV - H: " << std::fixed
                  << std::setprecision(2) << res.h << ", S: " << res.s << ", V: " << res.v
                  << std::endl;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }

      // Clock Commands
      case spike::CommandId::CLOCK_SLEEP: {
        spike::ClockSleepRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        std::cout << "MockServer: CLOCK_SLEEP - Milliseconds: " << req.milliseconds << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(req.milliseconds));
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::CLOCK_NOW: {
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
        spike::UInt64Response res{ static_cast<uint64_t>(now) };
        std::cout << "MockServer: CLOCK_NOW - Timestamp: " << res.value << std::endl;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }

      // Button Commands
      case spike::CommandId::BUTTON_IS_PRESSED: {
        spike::ButtonIsPressedRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        spike::BoolResponse res;
        // Toggle button state for demonstration
        if(req.target == spike::ButtonTarget::RIGHT)
          SpikeServerMock::button_pressed_state[0] = !SpikeServerMock::button_pressed_state[0];
        else if(req.target == spike::ButtonTarget::LEFT)
          SpikeServerMock::button_pressed_state[1] = !SpikeServerMock::button_pressed_state[1];
        else if(req.target == spike::ButtonTarget::CENTER)
          SpikeServerMock::button_pressed_state[2] = !SpikeServerMock::button_pressed_state[2];

        if(req.target == spike::ButtonTarget::RIGHT)
          res.value = SpikeServerMock::button_pressed_state[0];
        else if(req.target == spike::ButtonTarget::LEFT)
          res.value = SpikeServerMock::button_pressed_state[1];
        else if(req.target == spike::ButtonTarget::CENTER)
          res.value = SpikeServerMock::button_pressed_state[2];
        else
          res.value = false;  // Should not happen
        std::cout << "MockServer: BUTTON_IS_PRESSED - Target: " << static_cast<int>(req.target)
                  << ", Pressed: " << (res.value ? "true" : "false") << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }

      // Force Sensor Commands
      case spike::CommandId::FORCE_SENSOR_IS_PRESSED: {
        spike::ForceSensorIsPressedRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        spike::BoolResponse res;
        SpikeServerMock::force_sensor_pressed_state
            = bool_distribution(generator);  // Random boolean
        res.value = SpikeServerMock::force_sensor_pressed_state;
        std::cout << "MockServer: FORCE_SENSOR_IS_PRESSED - Threshold: " << req.threshold
                  << ", Pressed: " << (res.value ? "true" : "false") << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }
      case spike::CommandId::FORCE_SENSOR_GET_FORCE: {
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        SpikeServerMock::force_sensor_value = force_distribution(generator);  // Random force
        spike::DoubleResponse res{ SpikeServerMock::force_sensor_value };
        std::cout << "MockServer: FORCE_SENSOR_GET_FORCE - Value: " << res.value << std::endl;
        if(!sendData(client, &res, sizeof(res))) break;
        break;
      }

      // Display Commands
      case spike::CommandId::DISPLAY_SHOW_CHAR: {
        spike::DisplayShowCharRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        SpikeServerMock::display_char = req.character;
        std::cout << "MockServer: Display Char: '" << SpikeServerMock::display_char << "'"
                  << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::DISPLAY_SHOW_NUMBER: {
        spike::DisplayShowNumberRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        SpikeServerMock::display_number = req.number;
        std::cout << "MockServer: Display Number: "
                  << static_cast<int>(SpikeServerMock::display_number) << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }
      case spike::CommandId::DISPLAY_SCROLL_TEXT: {
        spike::DisplayScrollTextRequest req;
        if(!receiveData(client, &req, sizeof(req))) break;
        SpikeServerMock::display_scroll_text = req.text;
        std::cout << "MockServer: Display Scroll: '" << SpikeServerMock::display_scroll_text << "'"
                  << std::endl;
        if(!sendData(client, &response_header, sizeof(response_header))) break;
        break;
      }

      default:
        std::cerr << "MockServer: Unknown command ID: " << static_cast<uint16_t>(request.id)
                  << std::endl;
        response_header.value = false;
        sendData(client, &response_header, sizeof(response_header));
        break;
    }
  }

  delete client;
  std::cout << "MockServer: Client disconnected." << std::endl;
}

void SpikeServerMock::start()
{
  Socket server;
  if(!server.create()) {
    std::cerr << "MockServer: Failed to create socket." << std::endl;
    return;
  }
  if(!server.bind(8888)) {
    std::cerr << "MockServer: Failed to bind to port 8888." << std::endl;
    return;
  }
  if(!server.listen()) {
    std::cerr << "MockServer: Failed to listen on port 8888." << std::endl;
    return;
  }

  std::cout << "MockServer: Listening on port 8888..." << std::endl;

  while(true) {
    Socket* client = server.accept();
    if(client) {
      std::thread(handle_client, client).detach();
    }
  }
}
