/**
 * @file Pid.h
 * @brief PIDを計算するクラス
 * @author miyahara046
 */

#ifndef PID_H
#define PID_H

//   PIDゲインを保持する構造体
struct PidGain {
 public:
  double kp;  // Pゲイン
  double ki;  // Iゲイン
  double kd;  // Dゲイン

  /** コンストラクタ
   * @param _kp Pゲイン
   * @param _ki Iゲイン
   * @param _kd Dゲイン
   */
  PidGain(double _kp, double _ki, double _kd);
};

class Pid {
 public:
  /** コンストラクタ
   * @param _kp Pゲイン
   * @param _ki Iゲイン
   * @param _kd Dゲイン
   * @param _targetValue 目標値
   * @param _maxIntegral 累積積分値の最大値
   * @param _minIntegral 累積積分値の最小値
   */
  Pid(double _kp, double _ki, double _kd, double _targetValue, double _maxIntegral,
      double _minIntegral);

  /**
   * @brief PIDゲインを設定する
   * @param _kp Pゲイン
   * @param _ki Iゲイン
   * @param _kd Dゲイン
   */
  void setPidGain(double _kp, double _ki, double _kd);

  /**
   * @brief PIDを計算する
   * @param currentValue 現在値
   * @param delta 周期[ms](デフォルト値0.01[10ms]、省略可)
   * @return PIDの計算結果(操作量)
   */
  double calculatePid(double currentValue, double delta = 0.01);

 private:
  PidGain pidGain;
  double prevDeviation;           // 前回の偏差
  double integral;                // 偏差の累積
  double filteredDerivative = 0;  // フィルタされた微分項を保持する変数
  double targetValue;             // 目標値
  double maxIntegral = 100;       // 累積積分値の最大値
  double minIntegral = -100;      // 累積積分値の最小値
};

#endif