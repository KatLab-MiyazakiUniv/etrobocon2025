/**
 * @file   IMUController.h
 * @brief  IMU制御クラス
 * @author Hara1274
 */

#ifndef IMUCONTROLLER_H
#define IMUCONTROLLER_H

#include "IMU.h"
#include <array>
#include <chrono>
#include <thread>
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
   * @brief 補正済み角速度を取得する
   * @param angv 角速度格納用配列[3] (X, Y, Z軸)
   */
  void getCorrectedAngularVelocity(float angv[3]);

 private:
  /**
   * @brief 角度計算ループ処理
   */
  void angleCalculationLoop();

  /**
   * @brief IMUの傾きを補正するための行列を計算する
   */
  void calculateCorrectionMatrix();

  /**
   * @brief 補正行列を用いて角速度を補正する
   * @param raw 補正前の角速度
   * @param corrected 補正後の角速度
   */
  void correctAngularVelocity(const spikeapi::IMU::AngularVelocity& raw, float corrected[3]);

  /**
   * @brief IMUから生の角速度を取得する
   * @param angv 角速度格納用配列[3] (X, Y, Z軸)
   */
  void getRawAngularVelocity(float angv[3]);

  /**
   * @brief 傾き補正済みの角速度を取得する
   * @param angv 角速度格納用配列[3] (X, Y, Z軸)
   */
  void getCorrectedAngularVelocityWithoutOffset(float angv[3]);

  mutable std::mutex imuMutex;  // IMUデータのスレッドセーフなアクセス用ミューテックス

  spikeapi::IMU imu;                     // IMUインスタンス
  float offsetX = 0.0f;                  // X軸角速度オフセット値(deg/s)
  float offsetY = 0.0f;                  // Y軸角速度オフセット値(deg/s)
  float offsetZ = 0.0f;                  // Z軸角速度オフセット値(deg/s)
  float currentAngle = 0.0f;             // 計測結果用の現在角度(deg)
  double previousAngularVelocity = 0.0;  // 台形積分用：前回の角速度
  std::array<std::array<float, 3>, 3> correctionMatrix
      = { { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } } };  // 補正行列

  // 測定制御用メンバ変数
  std::thread angleCalculationThread;
  bool isCalculating = false;
  std::chrono::high_resolution_clock::time_point lastUpdateTime;
};

#endif