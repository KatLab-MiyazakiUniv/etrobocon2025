#ifndef SPIKE_SERVER_MOCK_H
#define SPIKE_SERVER_MOCK_H

#include <cstdint>
#include <string>
#include "SpikeCommand.h"

class SpikeServerMock {
 public:
  static void start();

  // State variables for mock values
  static int32_t right_motor_count;
  static int32_t left_motor_count;
  static int32_t arm_motor_count;
  static int32_t right_motor_power;
  static int32_t left_motor_power;
  static int32_t arm_motor_power;
  static double right_motor_speed;
  static double left_motor_speed;
  static int32_t reflection_value;
  static spike::HsvResponse hsv_value;
  static bool button_pressed_state[3];  // 0: RIGHT, 1: LEFT, 2: CENTER
  static bool force_sensor_pressed_state;
  static double force_sensor_value;
  static char display_char;
  static int8_t display_number;
  static std::string display_scroll_text;

 private:
};
#endif  // SPIKE_SERVER_MOCK_H