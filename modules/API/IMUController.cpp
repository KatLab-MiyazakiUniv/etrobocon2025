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
    isCalculating(false),
    shouldContinueCalculation(false),
    initialAccX(0.0f),
    initialAccY(0.0f),
    initialAccZ(0.0f),
    currentDistance(0.0f),
    currentVelocity(0.0),
    lastAcceleration(0.0),
    isDistanceCalc(false)
{
  // 補正行列を単位行列で初期化
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      correctionMatrix[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

void IMUController::setShouldContinueCalculation(bool value)
{
  shouldContinueCalculation = value;
}

bool IMUController::getShouldContinueCalculation() const
{
  return this->shouldContinueCalculation;
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

void IMUController::initializeOffset()
{
  float tempAngularVelocity[3];  // 角速度取得用の一時配列
  spikeapi::IMU::Acceleration acc;
  std::cout << "IMUオフセット計算を開始します。ロボットを静止状態に保ってください..." << std::endl;

  // 角度と角速度履歴を初期化
  resetAngle();
  resetDistance();
  // オフセット値を初期化
  offsetX = 0.0f;
  offsetY = 0.0f;
  offsetZ = 0.0f;
  initialAccX = 0.0f;
  initialAccY = 0.0f;
  initialAccZ = 0.0f;

  // オフセットの計算(1秒間で1000回測定して平均取る)
  for(int i = 0; i < 1000; i++) {
    getRawAngularVelocity(tempAngularVelocity);  // 角速度取得
    imu.getAcceleration(acc);                    // 加速度取得
    offsetX += tempAngularVelocity[0];
    offsetY += tempAngularVelocity[1];
    offsetZ += tempAngularVelocity[2];
    initialAccX += acc.x;
    initialAccY += acc.y;
    initialAccZ += acc.z;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 1ms待機
  }

  offsetX = offsetX / 1000.0f;
  offsetY = offsetY / 1000.0f;
  offsetZ = offsetZ / 1000.0f;
  initialAccX = initialAccX / 1000.0f;
  initialAccY = initialAccY / 1000.0f;
  initialAccZ = initialAccZ / 1000.0f;

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
  if(isCalculating) {
    std::cerr << "Warning: 角度計算は既に開始されています。" << std::endl;
    return;
  }
  isCalculating = true;

  // 角度を初期化（resetAngle()内での同一mutex再ロックを避けるため直接初期化）
  currentAngle = 0.0f;
  lastAngularVelocity = 0.0;

  // 現在時刻を記録
  lastUpdateTime = std::chrono::high_resolution_clock::now();

  // 角度更新用の専用スレッドを開始
  angleCalculationThread = std::thread([this]() { angleCalculationLoop(); });
}

void IMUController::stopAngleCalculation()
{
  // スコープを限定してjoin()前にmutexを解放
  {
    std::lock_guard<std::mutex> lock(imuMutex);
    if(!isCalculating) {
      std::cerr << "Warning: 角度計算は既に停止されています。" << std::endl;
      return;
    }
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
      // IMUのZ軸角速度は反時計回り時に正の値になるため、角速度を減算し、時計回りで角度が増加するようにする
      currentAngle -= (currentAngularVelocity + lastAngularVelocity) / 2.0 * deltaTime;

      // 角度を 0.0 <= angle < 360.0 の範囲に正規化を行い、-360.0~360.0の範囲に収める。
      currentAngle = fmod(currentAngle, 360.0);

      // 現在角度が負であれば, 360.0 を加算して0~360度の範囲に調整する。(例: -5.0 -> 355.0)
      // ループの２週目以降は、355から値が減少し続ける
      if(currentAngle < 0.0) {
        currentAngle += 360.0;
      }

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

double IMUController::getCorrectedXAxisAcceleration()
{
  spikeapi::IMU::Acceleration acc;
  imu.getAcceleration(acc);

  // 1. 静止時バイアス/重力成分の除去
  double rawAccX = acc.x - initialAccX;
  double rawAccY = acc.y - initialAccY;
  double rawAccZ = acc.z - initialAccZ;

  // 2. 傾き補正（補正行列の転置を適用し、計測された加速度を地面座標系に変換）
  double correctedAcc = correctionMatrix[0][0] * rawAccX + correctionMatrix[1][0] * rawAccY
                        + correctionMatrix[2][0] * rawAccZ;

  // 3. 閾値によるノイズ除去
  if(std::abs(correctedAcc) < ACCEL_THRESHOLD) {
    return 0.0;
  }

  return correctedAcc;
}

float IMUController::getDistance() const
{
  std::lock_guard<std::mutex> lock(imuMutex);
  return currentDistance;
}

void IMUController::resetDistance()
{
  std::lock_guard<std::mutex> lock(imuMutex);
  currentDistance = 0.0f;
  currentVelocity = 0.0;
  lastAcceleration = 0.0;
}

bool IMUController::isDistanceCalculating() const
{
  std::lock_guard<std::mutex> lock(imuMutex);
  return isDistanceCalc;
}

void IMUController::startDistanceCalculation()
{
  std::lock_guard<std::mutex> lock(imuMutex);
  if(isDistanceCalc) {
    std::cerr << "Warning: 距離計算は既に開始されています。" << std::endl;
    return;
  }
  isDistanceCalc = true;

  // 距離、速度、加速度履歴を初期化
  currentDistance = 0.0f;
  currentVelocity = 0.0;
  lastAcceleration = 0.0;

  // 現在時刻を記録
  lastDistUpdateTime = std::chrono::high_resolution_clock::now();

  // 距離更新用の専用スレッドを開始
  distanceCalculationThread = std::thread([this]() { distanceCalculationLoop(); });
}

void IMUController::stopDistanceCalculation()
{
  // スコープを限定してjoin()前にmutexを解放
  {
    std::lock_guard<std::mutex> lock(imuMutex);
    if(!isDistanceCalc) {
      std::cerr << "Warning: 距離計算は既に停止されています。" << std::endl;
      return;
    }
    isDistanceCalc = false;
  }

  if(distanceCalculationThread.joinable()) {
    distanceCalculationThread.join();
  }
}

void IMUController::distanceCalculationLoop()
{
  // 必要な変数をループ外で宣言
  const auto sleepDuration = std::chrono::milliseconds(10);
  auto currentTime = std::chrono::high_resolution_clock::now();
  double deltaTime, currentAcceleration;

  while(isDistanceCalc) {
    {
      std::lock_guard<std::mutex> lock(imuMutex);
      if(!isDistanceCalc) break;
    }

    // 測定開始時刻を記録
    auto measurementStartTime = std::chrono::high_resolution_clock::now();

    // IMUから補正済み加速度を取得 (X軸)
    currentAcceleration = getCorrectedXAxisAcceleration();

    // 測定終了時刻を記録
    auto measurementEndTime = std::chrono::high_resolution_clock::now();

    // 測定中間時刻を算出: (開始時刻 + 終了時刻) / 2
    currentTime = measurementStartTime + (measurementEndTime - measurementStartTime) / 2;

    {
      std::lock_guard<std::mutex> lock(imuMutex);
      deltaTime = std::chrono::duration<double>(currentTime - lastDistUpdateTime).count();

      // === 1. 速度の計算 (一回積分: 台形積分) ===
      // v += (a₁ + a₀)/2 × Δt
      currentVelocity += (currentAcceleration + lastAcceleration) / 2.0 * deltaTime;

      // ZUPTの応用: 加速度がゼロと判断されたら、速度も強制的にゼロに近づける（ドリフト抑制）
      if(std::abs(currentAcceleration) < ACCEL_THRESHOLD) {
        // 速度も閾値以下ならゼロにリセット
        if(std::abs(currentVelocity) < 1.0) {  // 例: 1 mm/s 以下の速度はゼロと見なす
          currentVelocity = 0.0;
        }
      }

      // === 2. 距離の計算 (二回積分: 台形積分) ===
      // x += (v₁ + v₀)/2 × Δt。ここでは速度の台形積分ではなく、単純な矩形積分 (v × Δt) を使用
      // 速度の台形積分は計算が複雑化するため、10ms間隔であれば速度×Δtで十分な精度が得られる
      currentDistance += currentVelocity * deltaTime;

      // 次回計算用に現在値を保存
      lastAcceleration = currentAcceleration;
      lastDistUpdateTime = currentTime;
    }

    // 10ms間隔を維持
    std::this_thread::sleep_for(sleepDuration);
  }
}