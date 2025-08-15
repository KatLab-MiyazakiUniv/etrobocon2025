/**
 * @file   SpikeServerMock.h
 * @brief  SPIKEサーバーのモッククラス
 * @author takuchi17
 */

#ifndef SPIKE_SERVER_MOCK_H
#define SPIKE_SERVER_MOCK_H

#include <cstdint>
#include <string>
#include "SpikeCommand.h"

class SpikeServerMock {
 public:
  /**
   * @brief モックサーバーを開始する
   */
  static void start();

  // State variables for mock values
  /**
   * @brief 右モーターのカウント
   */
  static int32_t right_motor_count;
  /**
   * @brief 左モーターのカウント
   */
  static int32_t left_motor_count;
  /**
   * @brief アームモーターのカウント
   */
  static int32_t arm_motor_count;
  /**
   * @brief 右モーターのパワー
   */
  static int32_t right_motor_power;
  /**
   * @brief 左モーターのパワー
   */
  static int32_t left_motor_power;
  /**
   * @brief アームモーターのパワー
   */
  static int32_t arm_motor_power;
  /**
   * @brief 右モーターの速度
   */
  static double right_motor_speed;
  /**
   * @brief 左モーターの速度
   */
  static double left_motor_speed;
  /**
   * @brief 反射値
   */
  static int32_t reflection_value;
  /**
   * @brief HSV値
   */
  static spike::HsvResponse hsv_value;
  /**
   * @brief ボタンの押下状態 (0: RIGHT, 1: LEFT, 2: CENTER)
   */
  static bool button_pressed_state[3];
  /**
   * @brief フォースセンサーの押下状態
   */
  static bool force_sensor_pressed_state;
  /**
   * @brief フォースセンサーの値
   */
  static float force_sensor_value;
  /**
   * @brief ディスプレイに表示する文字
   */
  static char display_char;
  /**
   * @brief ディスプレイに表示する数値
   */
  static int8_t display_number;
  /**
   * @brief ディスプレイにスクロール表示するテキスト
   */
  static std::string display_scroll_text;
};
#endif  // SPIKE_SERVER_MOCK_H