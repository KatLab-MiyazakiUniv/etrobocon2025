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

 private:
  const double targetSpeed;
  Pid rightPid;
  Pid leftPid;
  double rightMotorPower;
  double leftMotorPower;
  double prevRightTime;
  double prevLeftTime;
  Robot& robot;
  // PIDゲイン
  static constexpr double R_K_P = 0.00535;
  static constexpr double R_K_I = 0.00115;
  static constexpr double R_K_D = 0.000;
  static constexpr double L_K_P = 0.00578;
  static constexpr double L_K_I = 0.0008535;
  static constexpr double L_K_D = 0.000;
};
#endif