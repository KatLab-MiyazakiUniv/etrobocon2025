/**
 * @file   IMUShortestAngleRotation.cpp
 * @brief  IMU絶対角度の最短回頭動作
 * @author Hara127
 */

#include "IMUShortestAngleRotation.h"

#include "IMUController.h"
#include "MotorController.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

IMUShortestAngleRotation::IMUShortestAngleRotation(Robot& _robot, int _targetAngle, int _basePower,
                                                   const PidGain& _anglePidGain)
  : Rotation(_robot, true),
    targetAngle(_targetAngle),
    basePower(_basePower),
    anglePid(_anglePidGain.kp, _anglePidGain.ki, _anglePidGain.kd, 0.0),
    currentAngle(0.0f),
    angleError(0.0f)
{
}

void IMUShortestAngleRotation::prepare()
{
  // IMUから現在角度を取得する
  currentAngle = robot.getIMUControllerInstance().getAngle();

  // 目標角度への最短の角度を計算
  angleError = targetAngle - currentAngle;
  if(angleError > 180.0) {
    angleError -= 360.0;
  } else if(angleError < -180.0) {
    angleError += 360.0;
  }

  // 誤差の符号に応じて回頭方向と左右モータの符号を設定する
  isClockwise = angleError >= 0.0;
  leftSign = isClockwise ? 1 : -1;
  rightSign = isClockwise ? -1 : 1;
}

bool IMUShortestAngleRotation::isMetPreCondition()
{
  // 角度をチェック
  if(targetAngle < 0 || targetAngle >= 360) {
    std::cerr << "targetAngle=" << targetAngle << " は範囲外です。" << std::endl;
    return false;
  }

  auto& imuController = robot.getIMUControllerInstance();

  // IMUの角度計算が(ISコマンドで)開始されていなければエラー
  if(!imuController.getShouldContinueCalculation() || !imuController.isAngleCalculating()) {
    std::cerr << "IMU角度計算が開始されていません。IS,startで事前に開始してください。" << std::endl;
    return false;
  }

  return true;
}

bool IMUShortestAngleRotation::isMetContinuationCondition()
{
  // 現在の角度を取得
  currentAngle = robot.getIMUControllerInstance().getAngle();

  // 残りの最短の回頭角度を計算
  angleError = targetAngle - currentAngle;
  if(angleError > 180.0) {
    angleError -= 360.0;
  } else if(angleError < -180.0) {
    angleError += 360.0;
  }

  // 誤差の絶対値が許容値より大きい間は継続
  return std::abs(angleError) > TOLERANCE;
}

void IMUShortestAngleRotation::updateMotorControl()
{
  // PID制御により角度誤差から補正値を計算
  double pidCorrection = anglePid.calculatePid(angleError);

  // 基準パワー値にPID補正を適用
  double motorPower = basePower + pidCorrection;

  // 回頭する向きを決定
  isClockwise = angleError >= 0.0;
  leftSign = isClockwise ? 1 : -1;
  rightSign = isClockwise ? -1 : 1;

  // モータパワーを適用
  robot.getMotorControllerInstance().setLeftMotorPower(motorPower * leftSign);
  robot.getMotorControllerInstance().setRightMotorPower(motorPower * rightSign);

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
