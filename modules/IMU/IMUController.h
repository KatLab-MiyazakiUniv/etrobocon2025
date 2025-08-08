/**
 * @file   IMUController.h
 * @brief  IMU制御クラス
 * @author Hara1274
 */

#ifndef IMUCONTROLLER_H
#define IMUCONTROLLER_H

#include "IMU.h"
#include <chrono>
#include <thread>
#include <iostream>

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
   * @brief 角度計算ループ処理（標準版）
   */
  void angleCalculationLoop();
  
  /**
   * @brief 角度計算ループ処理（測定タイミング中心化版）
   */
  void angleCalculationLoopCentered();
  
  /**
   * @brief 角度計算ループ処理（台形積分版）
   */
  void angleCalculationLoopTrapezoidal();
  
  /**
   * @brief 角度計算ループ処理（台形積分+測定タイミング中心化版）
   */
  void angleCalculationLoopOptimal();
  spikeapi::IMU imu;      // IMUインスタンス
  float offsetX = 0.0f;  // X軸角速度オフセット値(deg/s)
  float offsetY = 0.0f;  // Y軸角速度オフセット値(deg/s)
  float offsetZ = 0.0f;  // Z軸角速度オフセット値(deg/s)
  float currentAngle = 0.0f;  // 計測結果用の現在角度(deg)
  double previousAngularVelocity = 0.0;  // 台形積分用：前回の角速度
  
  // 測定制御用メンバ変数
  std::thread angleCalculationThread;
  bool isCalculating = false;
  std::chrono::high_resolution_clock::time_point lastUpdateTime;
};

#endif