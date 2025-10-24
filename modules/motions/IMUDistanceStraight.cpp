/**
 * @file   IMUDistanceStraight.cpp
 * @brief  IMU角度補正を用いた目標距離まで直進するクラス
 * @author Hara1274
 */

#include "IMUDistanceStraight.h"
#include <thread>
#include <chrono>

IMUDistanceStraight::IMUDistanceStraight(Robot& _robot, double _targetDistance, double _targetSpeed,
                                         const PidGain& _anglePidGain)
  : Straight(_robot, _targetSpeed),
    targetDistance(_targetDistance),
    anglePid(_anglePidGain.kp, _anglePidGain.ki, _anglePidGain.kd, 0.0),
    targetAngle(0.0)
{
}

bool IMUDistanceStraight::isMetPreCondition()
{
  // targetSpeed値が0の場合は終了する
  if(targetSpeed == 0.0) {
    return false;
  }

  // targetDistance値が0以下の場合は終了する
  if(targetDistance <= 0.0) {
    return false;
  }

  // IMU角度計算が既に開始されている場合、それがコマンドによるものでなければエラー
  if(robot.getIMUControllerInstance().isAngleCalculating()
     && !robot.getIMUControllerInstance().shouldContinueCalculation()) {
    std::cerr << "IMU角度計算が既に開始されています。" << std::endl;
    return false;
  }

  return true;
}

void IMUDistanceStraight::prepare()
{
  // IMU角度計算がコマンドで開始されていなければ、この動作で計算を開始する
  if(!robot.getIMUControllerInstance().shouldContinueCalculation()) {
    robot.getIMUControllerInstance().startAngleCalculation();
  }

  // 呼び出し時の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // 走行前の角度を目標角度に設定
  targetAngle = robot.getIMUControllerInstance().getAngle();
}

bool IMUDistanceStraight::isMetContinuationCondition()
{
  // 現在の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  // 現在の走行距離が目標走行距離に達した場合falseを返す
  if((fabs(currentDistance - initialDistance) >= targetDistance)) {
    return false;
  }

  // 現在の走行距離が目標走行距離に達していなければ走行を続ける
  return true;
}

void IMUDistanceStraight::run()
{
  // 事前条件判定が真でないときは終了する
  if(!isMetPreCondition()) {
    return;
  }

  // 事前準備
  prepare();

  SpeedCalculator speedCalculator(robot, targetSpeed);

  // 継続条件を満たしている間繰り返す
  while(isMetContinuationCondition()) {
    // Power値を計算
    double currentRightPower = speedCalculator.calculateRightMotorPower();
    double currentLeftPower = speedCalculator.calculateLeftMotorPower();

    // 角度のズレを補正する
    double currentAngle = robot.getIMUControllerInstance().getAngle();
    double angleError = targetAngle - currentAngle;

    // 角度の誤差を-180度から180度の範囲に正規化
    if(angleError > 180.0) {
      angleError -= 360.0;
    } else if(angleError < -180.0) {
      angleError += 360.0;
    }

    double turningPower = anglePid.calculatePid(angleError, 0.01);

    // 後退時は補正方向を逆にする
    if(targetSpeed < 0) {
      turningPower = -turningPower;
    }

    // モーターにPower値をセット
    robot.getMotorControllerInstance().setRightMotorPower(currentRightPower + turningPower);
    robot.getMotorControllerİnstance().setLeftMotorPower(currentLeftPower - turningPower);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒待機
  }

  // モータを停止
  robot.getMotorControllerInstance().stopWheelsMotor();

  // この動作で角度計算を開始した場合のみ、計算を停止
  if(!robot.getIMUControllerInstance().shouldContinueCalculation()) {
    robot.getIMUControllerInstance().stopAngleCalculation();
  }
}
