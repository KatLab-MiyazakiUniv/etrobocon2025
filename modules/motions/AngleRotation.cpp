/**
 * @file   AngleRotation.cpp
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#include "AngleRotation.h"

AngleRotation::AngleRotation(Robot& _robot, int _targetAngle, double _speed, bool _isClockwise)
  : Rotation(_robot, _speed, _isClockwise),
    targetAngle(_targetAngle),
    targetLeftDistance(0.0),
    targetRightDistance(0.0),
    accumulatedAngle(0.0),
    lastTime(std::chrono::steady_clock::now())
{
}

void AngleRotation::prepare()
{
  MotorController& motorController = robot.getMotorControllerInstance();

  // 現在の走行距離を取得
  double initLeftMileage = Mileage::calculateWheelMileage(motorController.getLeftMotorCount());
  double initRightMileage = Mileage::calculateWheelMileage(motorController.getRightMotorCount());

  // 回頭距離 = π × TREAD(両輪間距離[mm]) × (角度 / 360) により各車輪の目標距離を算出
  double targetDistance = PI * TREAD * targetAngle / 360.0;

  // 目標走行距離を方向に応じて設定
  targetLeftDistance = initLeftMileage + targetDistance * leftSign;
  targetRightDistance = initRightMileage + targetDistance * rightSign;
}

bool AngleRotation::isMetPreCondition()
{
  // スピードが0以下なら終了
  if(speed <= 0.0) {
    std::cerr << "speed=" << speed << " は無効な値です" << std::endl;
    return false;
  }

  // 角度が0以下または360以上なら終了
  if(targetAngle <= 0 || targetAngle >= 360) {
    std::cerr << "targetAngle=" << targetAngle << " は範囲外です。" << std::endl;
    return false;
  }

  return true;
}

bool AngleRotation::isMetContinuationCondition()
{
  MotorController& motorController = robot.getMotorControllerInstance();

  // 現在の時間と前回の時間から経過秒数を取得
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  std::chrono::duration<double> dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastTime);
  lastTime = now;

  // IMUから角速度取得
  spikeapi::IMU::AngularVelocity angVel;
  robot.getIMUInstance().getAngularVelocity(angVel);

  // z軸角速度（deg/s）× 経過時間（s）で角度積分
  accumulatedAngle += std::abs(angVel.z * dt.count());

  // デバッグ出力（必要なら）
  std::cout << "[AngleRotation] accumulatedAngle = " << accumulatedAngle << std::endl;

  // 残りの回転に必要な走行距離を算出
  double diffLeftDistance
      = (targetLeftDistance - Mileage::calculateWheelMileage(motorController.getLeftMotorCount()))
        * leftSign;
  double diffRightDistance
      = (targetRightDistance - Mileage::calculateWheelMileage(motorController.getRightMotorCount()))
        * rightSign;
  // 目標距離に到達した場合
  if(diffLeftDistance <= 0.0 && diffRightDistance <= 0.0) {
    return false;
  }
  return true;
}