/**
 * @file   IMUController.cpp
 * @brief  IMU制御クラス
 * @author Hara1274 takuchi17
 */

#include "IMUController.h"

IMUController::IMUController()
  : imu(),
    offsetX(0.0f),
    offsetY(0.0f),
    offsetZ(0.0f),
    currentAngle(0.0f),
    lastAngularVelocity(0.0),
    isCalculating(false)
{
  // 補正行列を単位行列で初期化
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      correctionMatrix[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

void IMUController::getRawAngularVelocity(float angv[3])
{
  spikeapi::IMU::AngularVelocity ang;
  imu.getAngularVelocity(ang);
  angv[0] = ang.x;  // 生のX軸角速度
  angv[1] = ang.y;  // 生のY軸角速度
  angv[2] = ang.z;  // 生のZ軸角速度
}

double IMUController::getCorrectedZAxisAngularVelocity()
{
  spikeapi::IMU::AngularVelocity ang;
  imu.getAngularVelocity(ang);

  // 補正行列の転置を適用し測定された角速度を地面座標系に変換
  return correctionMatrix[0][2] * (ang.x - offsetX) + correctionMatrix[1][2] * (ang.y - offsetY)
         + correctionMatrix[2][2] * (ang.z - offsetZ);
}

void IMUController::calculateCorrectionMatrix()
{
  spikeapi::IMU::Acceleration acc;
  imu.getAcceleration(acc);  // 加速度取得（mm/s^2）

  // RotationMatrixクラスを使用して補正行列を計算
  RotationMatrix::calculateCorrectionMatrix(acc.x, acc.y, acc.z, correctionMatrix);
}

void IMUController::calculateOffset()
{
  float tempAngularVelocity[3];  // 角速度取得用の一時配列

  std::cout << "IMUオフセット計算を開始します。ロボットを静止状態に保ってください..." << std::endl;

  // 角度と角速度履歴を初期化
  resetAngle();

  // オフセット値を初期化
  offsetX = 0.0f;
  offsetY = 0.0f;
  offsetZ = 0.0f;

  // オフセットの計算(1秒間で1000回測定して平均取る)
  for(int i = 0; i < 1000; i++) {
    getRawAngularVelocity(tempAngularVelocity);  // 角速度取得
    offsetX += tempAngularVelocity[0];
    offsetY += tempAngularVelocity[1];
    offsetZ += tempAngularVelocity[2];
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1ms待機
  }

  offsetX = offsetX / 1000.0f;
  offsetY = offsetY / 1000.0f;
  offsetZ = offsetZ / 1000.0f;

  std::cout << "IMUオフセット計算が完了しました。" << std::endl;
}

float IMUController::getAngle() const
{
  std::lock_guard<std::mutex> lock(imuMutex);
  return currentAngle;
}

void IMUController::resetAngle()
{
  std::lock_guard<std::mutex> lock(imuMutex);
  currentAngle = 0.0f;
  lastAngularVelocity = 0.0;
}

void IMUController::startAngleCalculation()
{
  std::lock_guard<std::mutex> lock(imuMutex);
  // 既に計算中の場合は二重実行を防止
  if(isCalculating) return;
  isCalculating = true;

  // 角度を初期化（resetAngle()は呼ばず直接初期化）
  currentAngle = 0.0f;
  lastAngularVelocity = 0.0;

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
  const auto sleepDuration = std::chrono::milliseconds(10);
  auto currentTime = std::chrono::high_resolution_clock::now();
  double deltaTime, currentAngularVelocity;

  // 10ms間隔で角速度積分による角度計算を実行（台形積分+測定タイミング中心化版）
  while(isCalculating) {
    {
      std::lock_guard<std::mutex> lock(imuMutex);
      if(!isCalculating) break;
    }

    // 測定開始時刻を記録
    auto measurementStartTime = std::chrono::high_resolution_clock::now();

    // IMUから補正済み角速度を取得
    currentAngularVelocity = getCorrectedZAxisAngularVelocity();

    // 測定終了時刻を記録
    auto measurementEndTime = std::chrono::high_resolution_clock::now();

    // 測定中間時刻を算出: (開始時刻 + 終了時刻) / 2
    currentTime = measurementStartTime + (measurementEndTime - measurementStartTime) / 2;

    {
      std::lock_guard<std::mutex> lock(imuMutex);
      deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

      // 台形積分による角度更新: θ += (ω₁ + ω₀)/2 × Δt
      // 参考: https://garchiving.com/angular-from-angular-acceleration/
      currentAngle += (currentAngularVelocity + lastAngularVelocity) / 2.0 * deltaTime;

      // 次回計算用に現在値を保存（中間時刻と角速度）
      lastAngularVelocity = currentAngularVelocity;
      lastUpdateTime = currentTime;
    }

    // 10ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}

bool IMUController::isAngleCalculating() const
{
  std::lock_guard<std::mutex> lock(imuMutex);
  return isCalculating;
}