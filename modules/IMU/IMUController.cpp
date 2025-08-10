/**
 * @file   IMUController.cpp
 * @brief  IMU制御クラス
 * @author Hara1274
 */

#include "IMUController.h"
#include <chrono>
#include <thread>
#include <iostream>

IMUController::IMUController()
  : imu(),
    offsetX(0.0f),
    offsetY(0.0f),
    offsetZ(0.0f),
    currentAngle(0.0f),
    previousAngularVelocity(0.0),
    isCalculating(false)
{
}

void IMUController::calculateOffset()
{
  float angv[3];  // 角速度を格納のするための配列

  std::cout << "IMUオフセット計算を開始します。ロボットを静止状態に保ってください..." << std::endl;

  // オフセット値を初期化
  offsetX = 0.0f;
  offsetY = 0.0f;
  offsetZ = 0.0f;

  // オフセットの計算((1秒間で1000回測定して平均取る)
  for(int i = 0; i < 1000; i++) {
    getAngularVelocity(angv);  // 角速度取得
    offsetX += angv[0];
    offsetY += angv[1];
    offsetZ += angv[2];
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1ms待機
  }

  offsetX = offsetX / 1000.0f;
  offsetY = offsetY / 1000.0f;
  offsetZ = offsetZ / 1000.0f;

  std::cout << "IMUオフセット計算が完了しました。" << std::endl;
  std::cout << "オフセット値 - X: " << offsetX << ", Y: " << offsetY << ", Z: " << offsetZ << " deg/s" << std::endl;
}

void IMUController::getAngularVelocity(float angv[3])
{
  spikeapi::IMU::AngularVelocity ang;
  imu.getAngularVelocity(ang);
  angv[0] = ang.x;
  angv[1] = ang.y;
  angv[2] = ang.z;
}

float IMUController::getAngle() const
{
  return currentAngle;
}

void IMUController::resetAngle()
{
  currentAngle = 0.0f;
}

void IMUController::startAngleCalculation()
{
  // 既に計算中の場合は二重実行を防止
  if(isCalculating) return;
  isCalculating = true;

  // 現在時刻を記録
  lastUpdateTime = std::chrono::high_resolution_clock::now();

  // 角度更新用の専用スレッドを開始
  angleCalculationThread = std::thread([this]() { angleCalculationLoop(); });
}

void IMUController::stopAngleCalculation()
{
  if(!isCalculating) return;
  isCalculating = false;

  if(angleCalculationThread.joinable()) {
    angleCalculationThread.join();
  }
}

void IMUController::angleCalculationLoop()
{
  // 必要な変数をループ外で宣言
  float angv[3];
  const auto sleepDuration = std::chrono::milliseconds(1);
  auto currentTime = std::chrono::high_resolution_clock::now();
  double deltaTime, currentAngularVelocity;
  

  // 1ms間隔で角速度積分による角度計算を実行（台形積分+測定タイミング中心化版）
  while(isCalculating) {
    // 測定開始時刻を記録
    auto measurementStartTime = std::chrono::high_resolution_clock::now();

    // IMUから角速度を取得し、53度傾きとオフセットを補正
    getAngularVelocity(angv);
    currentAngularVelocity = (angv[2] - offsetZ) * COS_TILT_ANGLE
                           - (angv[0] - offsetX) * SIN_TILT_ANGLE;

    // 測定終了時刻を記録
    auto measurementEndTime = std::chrono::high_resolution_clock::now();

    // 測定中間時刻を算出: (開始時刻 + 終了時刻) / 2
    currentTime = measurementStartTime + (measurementEndTime - measurementStartTime) / 2;
    deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

    // 台形積分による角度更新: θ += (ω₁ + ω₀)/2 × Δt
    currentAngle += (currentAngularVelocity + previousAngularVelocity) / 2.0 * deltaTime;

    // 次回計算用に現在値を保存（中間時刻と角速度）
    previousAngularVelocity = currentAngularVelocity;
    lastUpdateTime = currentTime;

    // 1ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}

void IMUController::continuousAngleOutput()
{
  std::cout << "=== 手動テスト開始：角度の継続出力（30秒間） ===" << std::endl;
  std::cout << "走行体を手で回転させてください..." << std::endl;
  
  resetAngle();
  startAngleCalculation();
  
  auto startTime = std::chrono::high_resolution_clock::now();
  auto lastPrintTime = startTime;
  
  // 30秒間継続
  while(std::chrono::high_resolution_clock::now() - startTime < std::chrono::seconds(30)) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    
    // 0.1秒ごとに角度を取得して出力
    if(currentTime - lastPrintTime >= std::chrono::milliseconds(100)) {
      float currentAngle = getAngle();
      std::cout << "現在角度: " << currentAngle << " deg" << std::endl;
      lastPrintTime = currentTime;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  
  stopAngleCalculation();
  std::cout << "=== 手動テスト終了 ===" << std::endl;
}

