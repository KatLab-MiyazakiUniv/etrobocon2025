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
  };

  // 全てのレスポンスに共通のヘッダ
  struct Response {
    bool value;
  };

  // --- リクエスト用データ構造 ---

  struct MotorSetPowerRequest {
    MotorTarget target;
    int32_t power;
  };

  struct MotorSetSpeedRequest {
    MotorTarget target;
    double speed;
  };

  struct MotorGetRequest {
    MotorTarget target;
  };

  struct ButtonIsPressedRequest {
    ButtonTarget target;
  };

  struct ClockSleepRequest {
    uint32_t milliseconds;
  };

  struct DisplayShowCharRequest {
    char character;
  };

  struct DisplayShowNumberRequest {
    int8_t number;
  };

  struct DisplayScrollTextRequest {
    char text[64];
    int32_t speed;
  };

  struct ForceSensorIsPressedRequest {
    float threshold;
  };

  // --- レスポンス用データ構造 ---

  struct Int32Response {
    int32_t value;
  };

  struct UInt32Response {
    uint32_t value;
  };

  struct UInt64Response {
    uint64_t value;
  };

  struct DoubleResponse {
    double value;
  };

  struct BoolResponse {
    bool value;
  };

  struct HsvResponse {
    float h;
    float s;
    float v;
  };

}  // namespace spike

#endif  // SPIKE_COMMAND_H
