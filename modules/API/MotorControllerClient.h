/**
 * @file   MotorControllerClient.h
 * @brief  モータ制御に用いる関数をまとめたラッパークラス
 * @author nishijima515 takuchi17
 */
#ifndef MOTOR_CONTROLLER_CLIENT_H
#define MOTOR_CONTROLLER_CLIENT_H

#include "SystemInfo.h"  // WHEEL_RADIUS, PI, RAD_TO_DEG, DEG_TO_RADの定義を含む
#include "APIClient.h"   // Inherit from APIClient
#include "SpikeCommand.h"

class MotorControllerClient : public APIClient {
 public:
  /**
   * @brief Power値の上限
   */
  static constexpr int MOTOR_POWER_MAX = 100;

  /**
   * @brief Power値の下限
   */
  static constexpr int MOTOR_POWER_MIN = -100;

  /**
   * @brief コンストラクタ
   * @param spikeClient SpikeClientのインスタンス
   */
  explicit MotorControllerClient(SpikeClient& spikeClient);

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
   * @brief 右タイヤのモータに, 線速度から算出した回転速度をセット
   * @param speed 線速度（mm/秒）
   */
  void setRightMotorSpeed(double speed);

  /**
   * @brief 左タイヤのモータに, 線速度から算出した回転速度をセット
   * @param speed 線速度（mm/秒）
   */
  void setLeftMotorSpeed(double speed);

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
   * @brief アームモータを止めて角度を維持する
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
   * @brief 右タイヤの線速度を取得する
   * @return 右タイヤの線速度（mm/秒）
   */
  double getRightMotorSpeed();

  /**
   * @brief 左タイヤの線速度を取得する
   * @return 左タイヤの線速度（mm/秒）
   */
  double getLeftMotorSpeed();

 private:
  // SpikeClient& spikeClient; // Moved to base class APIClient

  /**
   * @brief モータに設定するpower値の制限
   * @param inputPower 入力されたpower値
   * @return 制限されたpower値
   */
  int limitPowerValue(int inputPower);
};

#endif