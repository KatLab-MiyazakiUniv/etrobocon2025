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
  : imu(), offsetX(0.0f), offsetY(0.0f), offsetZ(0.0f), currentAngle(0.0f), isMeasuring(false) {}

void IMUController::calculateOffset()
{
  float angv[3];  // IMU角速度 格納用配列

  // ユーザーに静止状態での実行を促すメッセージ
  std::cout << "IMUオフセット計算を開始します。ロボットを静止状態に保ってください..." << std::endl;

  // オフセット値を初期化
  offsetX = 0.0f;
  offsetY = 0.0f;
  offsetZ = 0.0f;

  // オフセット同定 (1秒間で1000回測定して平均取る)
  for(int i = 0; i < 1000; i++) {
    getAngularVelocity(angv);  // 角速度取得
    offsetX += angv[0];
    offsetY += angv[1];
    offsetZ += angv[2];
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1ms待機
  }

  // 平均値を計算してオフセットとして設定
  offsetX /= 1000.0f;
  offsetY /= 1000.0f;
  offsetZ /= 1000.0f;

  std::cout << "IMUオフセット計算が完了しました。" << std::endl;
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

void IMUController::startMeasurement()
{
  if (isMeasuring) return;
  
  std::cout << "角度測定を開始します..." << std::endl;
  isMeasuring = true;
  lastUpdateTime = std::chrono::high_resolution_clock::now();
  
  measurementThread = std::thread([this]() {
    while (isMeasuring) {
      auto currentTime = std::chrono::high_resolution_clock::now();
      auto deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();
      
      // 角速度を取得
      float angv[3];
      getAngularVelocity(angv);
      
      // Z軸角速度をオフセット補正して角度に積分
      currentAngle += (angv[2] - offsetZ) * deltaTime;
      
      lastUpdateTime = currentTime;
      
      // 1ms間隔で更新
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  });
}

void IMUController::stopMeasurement()
{
  if (!isMeasuring) return;
  
  std::cout << "角度測定を終了します..." << std::endl;
  isMeasuring = false;
  
  if (measurementThread.joinable()) {
    measurementThread.join();
  }
}
