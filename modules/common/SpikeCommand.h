/**
 * @file   SpikeCommand.h
 * @brief  SPIKE通信のコマンドIDとデータ構造の定義
 * @author takuchi17
 */

#ifndef SPIKE_COMMAND_H
#define SPIKE_COMMAND_H

#include <cstdint>

namespace spike {
  // コマンドID
  enum class CommandId : uint16_t {
    // Motor
    MOTOR_SET_POWER,
    MOTOR_SET_SPEED,
    MOTOR_STOP_WHEELS,
    MOTOR_BRAKE_WHEELS,
    MOTOR_STOP_ARM,
    MOTOR_HOLD_ARM,
    MOTOR_GET_COUNT,
    MOTOR_GET_POWER,
    MOTOR_GET_SPEED,

    // Color Sensor
    COLOR_SENSOR_GET_REFLECTION,
    COLOR_SENSOR_GET_COLOR_HSV,

    // Clock
    CLOCK_SLEEP,
    CLOCK_NOW,

    // Button
    BUTTON_IS_PRESSED,

    // Force Sensor
    FORCE_SENSOR_IS_PRESSED,
    FORCE_SENSOR_GET_FORCE,

    // Display
    DISPLAY_SHOW_CHAR,
    DISPLAY_SHOW_NUMBER,
    DISPLAY_SCROLL_TEXT,
  };

  // モーターのターゲット
  enum class MotorTarget : uint8_t {
    RIGHT,
    LEFT,
    ARM,
  };

  // ボタンのターゲット
  enum class ButtonTarget : uint8_t {
    RIGHT,
    LEFT,
    CENTER,
  };

  // カラーセンサーのHue値
  enum class ColorHue : uint16_t {
    RED = 0,
    ORANGE = 30,
    YELLOW = 60,
    GREEN = 120,
    SPRING_GREEN = 150,
    CYAN = 180,
    BLUE = 240,
    VIOLET = 270,
    MAGENTA = 300,
  };

  // 全てのリクエストに共通のヘッダ
  struct Request {
    CommandId id;
  } __attribute__((packed));

  // 全てのレスポンスに共通のヘッダ
  struct Response {
    bool value;
  } __attribute__((packed));

  // --- リクエスト用データ構造 ---

  struct MotorSetPowerRequest {
    MotorTarget target;
    int32_t power;
  } __attribute__((packed));

  struct MotorSetSpeedRequest {
    MotorTarget target;
    int32_t speed;  // [°/s]
  } __attribute__((packed));

  struct MotorGetRequest {
    MotorTarget target;
  } __attribute__((packed));

  struct ButtonIsPressedRequest {
    ButtonTarget target;
  } __attribute__((packed));

  struct ClockSleepRequest {
    uint64_t microseconds;
  } __attribute__((packed));

  struct DisplayShowCharRequest {
    char character;
  } __attribute__((packed));

  struct DisplayShowNumberRequest {
    int8_t number;
  } __attribute__((packed));

  struct DisplayScrollTextRequest {
    char text[64];
    uint32_t delay;  // [ms]
  } __attribute__((packed));

  struct ForceSensorIsPressedRequest {
    float threshold;
  } __attribute__((packed));

  // --- レスポンス用データ構造 ---

  struct Int32Response {
    int32_t value;
  } __attribute__((packed));

  struct UInt32Response {
    uint32_t value;
  } __attribute__((packed));

  struct UInt64Response {
    uint64_t value;
  } __attribute__((packed));

  struct FloatResponse {
    float value;
  } __attribute__((packed));

  struct DoubleResponse {
    double value;
  } __attribute__((packed));

  struct BoolResponse {
    bool value;
  } __attribute__((packed));

  struct HsvResponse {
    uint16_t h;  // 0-359
    uint8_t s;   // 0-100
    uint8_t v;   // 0-100
  } __attribute__((packed));

}  // namespace spike

#endif  // SPIKE_COMMAND_H
