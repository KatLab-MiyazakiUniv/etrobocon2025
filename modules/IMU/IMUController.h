/**
 * @file   IMUController.h
 * @brief  IMU制御クラス
 * @author Hara1274 takuchi17
 */

#ifndef IMUCONTROLLER_H
#define IMUCONTROLLER_H

#include "IMU.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include <mutex>

class IMUController {
 public:
  /**
   * @brief コンストラクタ
   */
  IMUController();

  /**
   * @brief オフセットを計算して設定する
   */
  void calculateOffset();

  /**
   * @brief 角度計算を開始する
   */
  void startAngleCalculation();

  /**
   * @brief 角度計算を終了する
   */
  void stopAngleCalculation();

  /**
   * @brief 計測結果の角度を取得する
   * @return 角度(deg)
   */
  float getAngle() const;

  /**
   * @brief 計測結果用の角度の値をリセットする
   */
  void resetAngle();

  /**
   * @brief 角速度を取得する
   * @param angv 角速度格納用配列[3] (X, Y, Z軸)
   */
  void getAngularVelocity(float angv[3]);

 private:
  /**
   * @brief 角度計算用のループ処理
   */
  void angleCalculationLoop();

  spikeapi::IMU imu;                                                 // IMUインスタンス
  float offsetX = 0.0f;                                              // X軸角速度オフセット値(deg/s)
  float offsetY = 0.0f;                                              // Y軸角速度オフセット値(deg/s)
  float offsetZ = 0.0f;                                              // Z軸角速度オフセット値(deg/s)
  float currentAngle = 0.0f;                                         // 計測結果用の現在角度(deg)
  double previousAngularVelocity = 0.0;                              // 台形積分用：前回の角速度
  std::thread angleCalculationThread;                                // 角度計算用のスレッド
  bool isCalculating = false;                                        // 角度計算実行中フラグ
  std::chrono::high_resolution_clock::time_point lastUpdateTime;     // 前回更新時刻
  static constexpr double TILT_ANGLE_RAD = -53.2573 * M_PI / 180.0;  // SPIKE傾き角度(rad)
  static constexpr double COS_TILT_ANGLE = cos(TILT_ANGLE_RAD);      // 傾き角度のcos値
  static constexpr double SIN_TILT_ANGLE = sin(TILT_ANGLE_RAD);      // 傾き角度のsin値
};

#endif