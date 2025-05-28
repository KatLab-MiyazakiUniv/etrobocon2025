/**
 * @file SpeedCalculator.h
 *  @brief 目標速度に対応するPWM値を算出するクラス
 *  @author CHIHAYATAKU
 */

#ifndef SpeedCalculator_H
#define SpeedCalculator_H

#include "Pid.h"
#include "Robot.h"
#include "SystemInfo.h"

class SpeedCalculator {
 public:
  /**
   * @brief 引数２つの場合のコンストラクタ．（2024版では使わないかもしれないが一応残しておく）
   * @param _targetSpeed 目標とする右タイヤ走行速度[mm/s]
   * @param _leftTargetSpeed 目標とする左タイヤ走行速度[mm/s]
   */
  SpeedCalculator(Robot& _robot, double _targetSpeed);

  /**
   * @brief 目標とする走行速度に相当する右車輪のPWM値を算出する
   * @return 走行速度に相当する右タイヤのPWM値
   */
  double calculateRightMotorPowor();

  /**
   * @brief 目標とする走行速度に相当する左車輪のPWM値を算出する
   * @return 走行速度に相当する左タイヤのPWM値
   */
  double calculateLeftMotorPowor();

 private:
  const double targetSpeed;
  Pid rightPid;
  Pid leftPid;
  double rightMotorPower;
  double leftMotorPower;
  int prevRightTime;
  int prevLeftTime;
  Robot& robot;
  // 回頭以外のPIDゲイン
  static constexpr double K_P = 0.004;
  static constexpr double K_I = 0.0000005;
  static constexpr double K_D = 0.0007;
};
#endif