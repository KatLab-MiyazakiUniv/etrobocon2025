/**
 * @file SpeedCalculator.h
 * @brief 目標速度に対応するPWM値を算出するクラス
 * @author miyahara046 HaruArima08
 */

#ifndef SPEED_CALCULATOR_H
#define SPEED_CALCULATOR_H

#include "Pid.h"
#include "Robot.h"
#include "SystemInfo.h"

class SpeedCalculator {
 public:
  /**
   * @brief speedCalculatorのコンストラクタ
   * @param _robot Robotクラスのインスタンス
   * @param _targetSpeed 目標とするタイヤ走行速度[mm/s]
   */
  SpeedCalculator(Robot& _robot, double _targetSpeed);

  /**
   * @brief 目標とする走行速度に相当する右車輪のPower値を算出する
   * @return 走行速度に相当する右タイヤのPower値
   */
  double calculateRightMotorPower();

  /**
   * @brief 目標とする走行速度に相当する左車輪のPower値を算出する
   * @return 走行速度に相当する左タイヤのPower値
   */
  double calculateLeftMotorPower();

  /**
   * @brief 右タイヤのPIDゲインを設定する
   * @param rightKp 比例ゲイン
   * @param rightKi 積分ゲイン
   * @param rightKd 微分ゲイン
   */
  void setRightSpeedPidGain(double rightKp, double rightKi, double rightKd);

  /**
   * @brief 左タイヤのPIDゲインを設定する
   * @param leftKp 比例ゲイン
   * @param leftKi 積分ゲイン
   * @param leftKd 微分ゲイン
   */
  void setLeftSpeedPidGain(double leftKp, double leftKi, double leftKd);

  /**
   * @brief 両方タイヤのPIDゲインを設定する
   * @param kp 比例ゲイン
   * @param ki 積分ゲイン
   * @param kd 微分ゲイン
   */
  void setSpeedPidGain(double rightKp, double rightKi, double rightKd, double leftKp, double leftKi,
                       double leftKd);

 private:
  const double targetSpeed;
  Pid rightPid;
  Pid leftPid;
  double rightMotorPower;
  double leftMotorPower;
  double prevRightTime;
  double prevLeftTime;
  Robot& robot;
  // PIDゲインのデフォルト値
  static constexpr double K_P = 0.005;
  static constexpr double K_I = 0.0002;
  static constexpr double K_D = 0.000;
};
#endif