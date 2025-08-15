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
  const double targetSpeed;  // 目標とするタイヤ走行速度
  Pid rightPid;              // 右タイヤのPID制御器
  Pid leftPid;               // 左タイヤのPID制御器
  double rightMotorPower;    // 右タイヤのPower値
  double leftMotorPower;     // 左タイヤのPower値
  double prevRightTime;      // 前回の右タイヤの時刻
  double prevLeftTime;       // 前回の左タイヤの時刻
  Robot& robot;              // Robotクラスのインスタンス
  // PIDゲイン
  static constexpr double RIGHT_K_P = 0.00535;   // 右タイヤのPゲイン
  static constexpr double RIGHT_K_I = 0.00115;   // 右タイヤのIゲイン
  static constexpr double RIGHT_K_D = 0.000;     // 右タイヤのDゲイン
  static constexpr double LEFT_K_P = 0.00578;    // 左タイヤのPゲイン
  static constexpr double LEFT_K_I = 0.0008535;  // 左タイヤのIゲイン
  static constexpr double LEFT_K_D = 0.000;      // 左タイヤのDゲイン
};
#endif