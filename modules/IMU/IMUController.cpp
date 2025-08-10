/**
 * @file   IMUController.cpp
 * @brief  IMU制御クラス
 * @author Hara1274
 */

#include "IMUController.h"
#include <chrono>
#include <cmath>
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
  calculateCorrectionMatrix();
  float angv[3];  // 角速度を格納のするための配列

  std::cout << "IMUオフセット計算を開始します。ロボットを静止状態に保ってください..." << std::endl;

  // オフセット値を初期化
  offsetX = 0.0f;
  offsetY = 0.0f;
  offsetZ = 0.0f;

  // オフセットの計算((1秒間で1000回測定して平均取る)
  for(int i = 0; i < 1000; i++) {
    getCorrectedAngularVelocityWithoutOffset(angv);  // 補正済みの角速度取得
    offsetX += angv[0];
    offsetY += angv[1];
    offsetZ += angv[2];
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1ms待機
  }

  offsetX = offsetX / 1000.0f;
  offsetY = offsetY / 1000.0f;
  offsetZ = offsetZ / 1000.0f;

  std::cout << "IMUオフセット計算が完了しました。" << std::endl;
}

float IMUController::getAngle() const
{
  std::lock_guard<std::mutex> lock(imuMutex);  // ミューテックスで保護
  return currentAngle;
}

void IMUController::resetAngle()
{
  std::lock_guard<std::mutex> lock(imuMutex);  // ミューテックスで保護
  currentAngle = 0.0f;
}

void IMUController::startAngleCalculation()
{
  std::lock_guard<std::mutex> lock(imuMutex);  // ミューテックスで保護
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
  {
    std::lock_guard<std::mutex> lock(imuMutex);
    if(!isCalculating) return;
    isCalculating = false;
  }

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
    {
      std::lock_guard<std::mutex> lock(imuMutex);
      if(!isCalculating) break;  // 停止指示があれば終了
    }
    // 測定開始時刻を記録
    auto measurementStartTime = std::chrono::high_resolution_clock::now();

    // IMUからX軸角速度を取得しオフセット補正
    getCorrectedAngularVelocity(angv);
    currentAngularVelocity = angv[2];  // Z軸角速度を使用

    // 測定終了時刻を記録
    auto measurementEndTime = std::chrono::high_resolution_clock::now();

    // 測定中間時刻を算出: (開始時刻 + 終了時刻) / 2
    currentTime = measurementStartTime + (measurementEndTime - measurementStartTime) / 2;
    {
      std::lock_guard<std::mutex> lock(imuMutex);  // ミューテックスで保護
      deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

      // 台形積分による角度更新: θ += (ω₁ + ω₀)/2 × Δt
      currentAngle += (currentAngularVelocity + previousAngularVelocity) / 2.0 * deltaTime;

      // 次回計算用に現在値を保存（中間時刻と角速度）
      previousAngularVelocity = currentAngularVelocity;
      lastUpdateTime = currentTime;
    }

    // 1ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}

void IMUController::calculateCorrectionMatrix()
{
  spikeapi::IMU::Acceleration acc;
  imu.getAcceleration(acc);  // 加速度取得（mm/s^2）

  // 正規化
  float norm = std::sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
  float gx = acc.x / norm;
  float gy = acc.y / norm;
  float gz = acc.z / norm;

  // 理想の重力方向（Z軸）
  float ez[3] = { 0.0f, 0.0f, 1.0f };

  // 回転軸 v = ez × g
  float vx = ez[1] * gz - ez[2] * gy;  // 0*gz - 1*gy = -gy
  float vy = ez[2] * gx - ez[0] * gz;  // 1*gx - 0*gz = gx
  float vz = ez[0] * gy - ez[1] * gx;  // 0*gy - 0*gx = 0

  // 回転角 θ = acos(ez・g)
  float dot = ez[0] * gx + ez[1] * gy + ez[2] * gz;
  float theta = std::acos(dot);

  // 回転軸を正規化
  float v_norm = std::sqrt(vx * vx + vy * vy + vz * vz);
  if(v_norm < 1e-6) {
    // 傾きなし（単位行列をセット）
    correctionMatrix = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } } };
    std::cout << "傾き無し" << std::endl;
    return;
  }
  vx /= v_norm;
  vy /= v_norm;
  vz /= v_norm;

  float c = std::cos(theta);
  float s = std::sin(theta);
  float t = 1 - c;

  correctionMatrix = { { { t * vx * vx + c, t * vx * vy - s * vz, t * vx * vz + s * vy },
                         { t * vx * vy + s * vz, t * vy * vy + c, t * vy * vz - s * vx },
                         { t * vx * vz - s * vy, t * vy * vz + s * vx, t * vz * vz + c } } };
}

// 補正行列を使って角速度ベクトルを補正する例
void IMUController::correctAngularVelocity(const spikeapi::IMU::AngularVelocity& raw,
                                           float corrected[3])
{
  corrected[0] = correctionMatrix[0][0] * raw.x + correctionMatrix[0][1] * raw.y
                 + correctionMatrix[0][2] * raw.z;
  corrected[1] = correctionMatrix[1][0] * raw.x + correctionMatrix[1][1] * raw.y
                 + correctionMatrix[1][2] * raw.z;
  corrected[2] = correctionMatrix[2][0] * raw.x + correctionMatrix[2][1] * raw.y
                 + correctionMatrix[2][2] * raw.z;
}

void IMUController::getRawAngularVelocity(float angv[3])
{
  spikeapi::IMU::AngularVelocity ang;
  imu.getAngularVelocity(ang);
  angv[0] = ang.x;
  angv[1] = ang.y;
  angv[2] = ang.z;
}

void IMUController::getCorrectedAngularVelocityWithoutOffset(float angv[3])
{
  float raw[3];
  getRawAngularVelocity(raw);
  correctAngularVelocity(spikeapi::IMU::AngularVelocity{ raw[0], raw[1], raw[2] }, angv);
}

// 補正済み角速度を返すpublicメソッド
void IMUController::getCorrectedAngularVelocity(float angv[3])
{
  float raw[3];
  getRawAngularVelocity(raw);

  correctAngularVelocity(spikeapi::IMU::AngularVelocity{ raw[0], raw[1], raw[2] }, angv);

  // オフセット（バイアス）を引く
  angv[0] -= offsetX;
  angv[1] -= offsetY;
  angv[2] -= offsetZ;
}