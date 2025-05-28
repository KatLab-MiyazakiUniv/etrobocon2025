/**
 * @file SpeedCalculator.h
 *  @brief 目標速度に対応するPWM値を算出するクラス
 *  @author miyahara046
 */

#ifndef SpeedCalculator_H
#define SpeedCalculator_H

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
  int prevRightTime;
  int prevLeftTime;
  Robot& robot;
  // PIDゲイン
  static constexpr double K_P = 0.004;
  static constexpr double K_I = 0.0000005;
  static constexpr double K_D = 0.0007;
};
#endif