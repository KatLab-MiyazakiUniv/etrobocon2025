/**
 * @file   IMUController.h
 * @brief  IMU制御クラス
 * @author Hara1274 takuchi17
 */

#ifndef IMUCONTROLLER_H
#define IMUCONTROLLER_H

#include "IMU.h"
#include "RotationMatrix.h"
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
   * @brief 生の角速度を取得する（補正なし）
   * @param angv 生の角速度格納用配列[3] (X, Y, Z軸)
   */
  void getRawAngularVelocity(float angv[3]);

  /**
   * @brief 3D回転補正行列を計算する
   */
  void calculateCorrectionMatrix();

  /**
   * @brief オフセットを計算して設定する
   */
  void calculateOffset();

  /**
   * @brief 補正済みZ軸角速度を取得する（傾き補正+オフセット補正）
   * @return 補正済みZ軸角速度(deg/s)
   */
  double getCorrectedZAxisAngularVelocity();

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
   * @brief 角度計算を開始する
   */
  void startAngleCalculation();

  /**
   * @brief 角度計算を終了する
   */
  void stopAngleCalculation();

  /**
   * @brief 角度計算が実行中かどうかを取得する
   * @return true: 実行中, false: 停止中
   */
  bool isAngleCalculating() const;

 private:
  /**
   * @brief 角度計算用のループ処理
   */
  void angleCalculationLoop();

  spikeapi::IMU imu;                 // IMUインスタンス
  float correctionMatrix[3][3];      // 3D回転補正行列
  float cosSpikeInclination = 1.0f;  // SPIKE傾斜角のcos値
  float sinSpikeInclination = 0.0f;  // SPIKE傾斜角のsin値
  float offsetX = 0.0f;              // X軸角速度オフセット値(deg/s)
  float offsetY = 0.0f;              // Y軸角速度オフセット値(deg/s)
  float offsetZ = 0.0f;              // Z軸角速度オフセット値(deg/s)
  float spikeInclination = 0.0f;     // SPIKE設置傾斜角(rad)
  float currentAngle = 0.0f;         // 計測結果用の現在角度(deg)
  double lastAngularVelocity = 0.0;  // 台形積分用：前回の角速度
  std::chrono::high_resolution_clock::time_point lastUpdateTime;  // 前回更新時刻
  mutable std::mutex imuMutex;  // IMUデータのスレッドセーフなアクセス用ミューテックス
  std::thread angleCalculationThread;  // 角度計算用のスレッド
  bool isCalculating = false;          // 角度計算実行中フラグ
};

#endif