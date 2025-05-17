/**
 * @file Pid.cpp
 * @brief PIDを計算するクラス
 * @author miyahara046
 */

#include "Pid.h"
PidGain::PidGain(double _kp, double _ki, double _kd)
  // pidゲインが負の値にならないようにする
  : kp(_kp < 0 ? 0 : _kp), ki(_ki < 0 ? 0 : _ki), kd(_kd < 0 ? 0 : _kd)
{
}

Pid::Pid(double _kp, double _ki, double _kd, double _targetValue, double _maxIntegral,
         double _minIntegral)
  : pidGain(_kp, _ki, _kd),
    prevDeviation(0.0),
    integral(0.0),
    targetValue(_targetValue),
    maxIntegral(_maxIntegral),
    minIntegral(_minIntegral)
{
}

void Pid::setPidGain(double _kp, double _ki, double _kd)
{
  // pidゲインが負の値にならないようにする
  pidGain.kp = _kp < 0 ? 0 : _kp;
  pidGain.ki = _ki < 0 ? 0 : _ki;
  pidGain.kd = _kd < 0 ? 0 : _kd;
}

double Pid::calculatePid(double currentValue, double delta)
{
  // 0除算を避けるために0の場合はデフォルト周期0.01とする
  if(delta == 0) delta = 0.01;

  // 現在の目標値との偏差を求める
  double currentDeviation = targetValue - currentValue;
  // 積分の処理を行う
  integral += (currentDeviation + prevDeviation) * delta / 2;
  // 累積する積分値の大きさ制限
  if(integral > maxIntegral)
    integral = maxIntegral;
  else if(integral < minIntegral)
    integral = minIntegral;
  // 微分の処理を行う
  double currentDerivative = (currentDeviation - prevDeviation) / delta;
  /**
   * 微分項にローパスフィルタを適用
   * 偏差が大きい際に過大な変化量を一気に与えず
   * 滑らかな変化にし、機体の暴走を防ぐため
   */
  filteredDerivative = alpha * currentDerivative + (1 - alpha) * filteredDerivative;

  // 前回の偏差を更新する
  prevDeviation = currentDeviation;

  // P制御の計算を行う
  double p = pidGain.kp * currentDeviation;
  // I制御の計算を行う
  double i = pidGain.ki * integral;
  // D制御の計算を行う
  double d = pidGain.kd * filteredDerivative;

  // 操作量 = P制御 + I制御 + D制御
  return (p + i + d);
}