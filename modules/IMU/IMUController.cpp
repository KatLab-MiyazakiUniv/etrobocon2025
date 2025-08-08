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
  double deltaTime, correctedAngularVelocity;

  // 1ms間隔で角速度積分による角度計算を実行
  while(isCalculating) {
    // 前回からの経過時間を算出
    currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

    // IMUからZ軸角速度を取得しオフセット補正
    getAngularVelocity(angv);
    correctedAngularVelocity = angv[2] - offsetZ;

    // 角速度×時間で角度を更新
    // とても参考になるサイト　https://garchiving.com/angular-from-angular-acceleration/
    currentAngle += correctedAngularVelocity * deltaTime;

    // 次回計算用に現在時刻を保存
    lastUpdateTime = currentTime;

    // 1ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}

void IMUController::angleCalculationLoopCentered()
{
  // 必要な変数をループ外で宣言
  float angv[3];
  const auto sleepDuration = std::chrono::milliseconds(1);
  auto currentTime = std::chrono::high_resolution_clock::now();
  double deltaTime, correctedAngularVelocity;

  // 1ms間隔で角速度積分による角度計算を実行（測定タイミング中心化版）
  while(isCalculating) {
    // 測定開始時刻を記録
    auto measurementStartTime = std::chrono::high_resolution_clock::now();

    // IMUからZ軸角速度を取得しオフセット補正
    getAngularVelocity(angv);
    correctedAngularVelocity = angv[2] - offsetZ;

    // 測定終了時刻を記録
    auto measurementEndTime = std::chrono::high_resolution_clock::now();

    // 測定中間時刻を算出: (開始時刻 + 終了時刻) / 2
    // IMU読み取りは瞬間的ではなく数十〜数百μs要するため、
    // 測定期間の中点を実際のデータ取得時刻として扱い積分精度を向上
    currentTime = measurementStartTime + (measurementEndTime - measurementStartTime) / 2;
    deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

    // 角速度×時間で角度を更新
    // とても参考になるサイト　https://garchiving.com/angular-from-angular-acceleration/
    currentAngle += correctedAngularVelocity * deltaTime;

    // 次回計算用に中間時刻を保存
    lastUpdateTime = currentTime;

    // 1ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}

void IMUController::angleCalculationLoopTrapezoidal()
{
  // 必要な変数をループ外で宣言
  float angv[3];
  const auto sleepDuration = std::chrono::milliseconds(1);
  auto currentTime = std::chrono::high_resolution_clock::now();
  double deltaTime, currentAngularVelocity;

  // 1ms間隔で角速度積分による角度計算を実行（台形積分版）
  while(isCalculating) {
    // 前回からの経過時間を算出
    currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

    // IMUからZ軸角速度を取得しオフセット補正
    getAngularVelocity(angv);
    currentAngularVelocity = angv[2] - offsetZ;

    // 台形積分による角度更新: θ += (ω₁ + ω₀)/2 × Δt
    // より高精度な数値積分（前回と今回の角速度の平均値を使用）
    currentAngle += (currentAngularVelocity + previousAngularVelocity) / 2.0 * deltaTime;

    // 次回計算用に現在値を保存
    previousAngularVelocity = currentAngularVelocity;
    lastUpdateTime = currentTime;

    // 1ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}

void IMUController::angleCalculationLoopOptimal()
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

    // IMUからZ軸角速度を取得しオフセット補正
    getAngularVelocity(angv);
    currentAngularVelocity = angv[2] - offsetZ;

    // 測定終了時刻を記録
    auto measurementEndTime = std::chrono::high_resolution_clock::now();

    // 測定中間時刻を算出: (開始時刻 + 終了時刻) / 2
    // IMU読み取りは瞬間的ではなく数十〜数百μs要するため、
    // 測定期間の中点を実際のデータ取得時刻として扱い積分精度を向上
    currentTime = measurementStartTime + (measurementEndTime - measurementStartTime) / 2;
    deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

    // 台形積分による角度更新: θ += (ω₁ + ω₀)/2 × Δt
    // 高精度数値積分と物理的整合性を両立した最適化手法
    currentAngle += (currentAngularVelocity + previousAngularVelocity) / 2.0 * deltaTime;

    // 次回計算用に現在値を保存（中間時刻と角速度）
    previousAngularVelocity = currentAngularVelocity;
    lastUpdateTime = currentTime;

    // 1ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}
