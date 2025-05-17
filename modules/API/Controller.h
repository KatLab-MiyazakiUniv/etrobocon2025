/**
 * @file Controller.h
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author nishijima515
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Motor.h"

class Controller {
 public:
  /** Power値の上限 */
  static constexpr int MOTOR_POWER_MAX = 100;

  /** Power値の下限 */
  static constexpr int MOTOR_POWER_MIN = -100;

  /**
   * コンストラクタ
   */
  Controller();

  /**
   * @brief タイヤのモータにPower値をセット
   * @param power Power値
   */
  void setRightMotorPower(const int power);
  void setLeftMotorPower(const int power);

  /**
   * @brief タイヤのモータのpower値をリセット
   */
  void resetRightMotorPower();
  void resetLeftMotorPower();
  void resetWheelsMotorPower();

  /**
   * @brief タイヤのモータを停止する
   */
  void stopWheelsMotor();

  /**
   * @brief アームのモータにpower値をセット
   * @param power power値
   */
  void setArmMotorPower(const int power);

  /**
   * @brief アームのモータのpower値をリセット
   */
  void resetArmMotorPower();

  /**
   * @brief アームのモータを停止する
   */
  void stopArmMotor();

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

 private:
  Motor rightWheel;
  Motor leftWheel;
  Motor armMotor;
  static int powerOfRightWheel;  // 右タイヤpower
  static int powerOfLeftWheel;   // 左タイヤpower
  static int powerOfArm;         // アームpower

  /**
   * @brief モータに設定するpower値の制限
   * @param inputpower 入力されたpower値
   * @return 制限されたpower値
   */
  int limitPowerValue(const int inputPower);
};

#endif