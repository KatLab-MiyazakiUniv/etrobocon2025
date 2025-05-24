/**
 * @file MotorController.h
 * @brief モータ制御に用いる関数をまとめたラッパークラス
 * @author nishijima515
 */
#ifndef MOTOR_MOTORCONTROLLER_H
#define MOTOR_MOTORCONTROLLER_H

#include "Motor.h"
#include <cstdint>

using namespace spikeapi;

class MotorController {
 public:
  /** Power値の上限 */
  static constexpr int MOTOR_POWER_MAX = 100;

  /** Power値の下限 */
  static constexpr int MOTOR_POWER_MIN = -100;

  /**
   * コンストラクタ
   */
  MotorController();

  /**
   * @brief 右タイヤのモータにPower値をセット
   * @param power Power値
   */
  void setRightMotorPower(int power);

  /**
   * @brief 左タイヤのモータにPower値をセット
   * @param power Power値
   */
  void setLeftMotorPower(int power);

  /**
   * @brief 右タイヤのモータのpower値を0にリセット
   */
  void resetRightMotorPower();

  /**
   * @brief 左タイヤのモータのpower値を0にリセット
   */
  void resetLeftMotorPower();

  /**
   * @brief 両タイヤのモータのpower値を0にリセット
   */
  void resetWheelsMotorPower();

  /**
   * @brief 右タイヤのモータに回転速度をセット
   * @param speed 回転速度（°/秒）
   */
  void setRightMotorSpeed(int speed);

  /**
   * @brief 左タイヤのモータに回転速度をセット
   * @param speed 回転速度（°/秒）
   */
  void setLeftMotorSpeed(int speed);

  /**
   * @brief 両タイヤのモータを停止する
   */
  void stopWheelsMotor();

  /**
   * @brief ブレーキをかけてタイヤのモータを停止する
   */
  void brakeWheelsMotor();

  /**
   * @brief アームのモータにpower値をセット
   * @param power power値
   */
  void setArmMotorPower(int power);

  /**
   * @brief アームのモータのpower値を0にリセット
   */
  void resetArmMotorPower();

  /**
   * @brief アームのモータを停止する
   */
  void stopArmMotor();

  /**
   * アームモータを止めて角度を維持する

   */
  void holdArmMotor();

  /**
   * @brief 右モータの角位置を取得する
   * @return 右モータの角位置（°）
   */
  int32_t getRightMotorCount();

  /**
   * @brief 左モータの角位置を取得する
   * @return 左モータの角位置（°）
   */
  int32_t getLeftMotorCount();

  /**
   * @brief アームモータの角位置を取得する
   * @return アームモータの角位置（°）
   */
  int32_t getArmMotorCount();

  /**
   * @brief 右タイヤのpower値を取得する
   * @return 右タイヤのpower値
   */
  int getRightMotorPower();

  /**
   * @brief 左タイヤのpower値を取得する
   * @return 左タイヤのpower値
   */
  int getLeftMotorPower();

  /**
   * @brief アームモータのpower値を取得する
   * @return アームモータのpower値
   */
  int getArmMotorPower();

  /**
   * @brief 右タイヤの回転速度を取得する
   * @return 右タイヤの回転速度（°/秒）
   */
  int32_t getRightMotorSpeed();

  /**
   * @brief 左タイヤの回転速度を取得する
   * @return 左タイヤの回転速度（°/秒）
   */
  int32_t getLeftMotorSpeed();

 private:
  Motor rightWheel;  // 右タイヤモータのインスタンス
  Motor leftWheel;   // 左タイヤモータのインスタンス
  Motor armMotor;    // アームモータのインスタンス

  /**
   * @brief モータに設定するpower値の制限
   * @param inputpower 入力されたpower値
   * @return 制限されたpower値
   */
  int limitPowerValue(int inputPower);
};

#endif