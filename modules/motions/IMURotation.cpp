/**
 * @file   IMURotation.cpp
 * @brief  IMU角度指定回頭動作
 * @author Hara1274
 */

#include "IMURotation.h"

IMURotation::IMURotation(Robot& _robot, int _targetAngle, double _power, bool _isClockwise,
                         const PidGain& _pidGain)
  : Rotation(_robot, _power, _isClockwise),
    targetAngle(_targetAngle),
    power(_power),
    pidGain(_pidGain),
    pid(_pidGain.kp, _pidGain.ki, _pidGain.kd, _targetAngle)
{
}

void IMURotation::prepare()
{
  // IMU角度計算開始
  robot.getIMUControllerInstance().startAngleCalculation();
}

bool IMURotation::isMetPreCondition()
{
  return true;
}

bool IMURotation::isMetContinuationCondition()
{
  // 現在角度を取得
  float currentAngle = robot.getIMUControllerInstance().getAngle();

  // IMUは時計回りをマイナス、反時計回りをプラスで出力
  if(isClockwise) {
    // 時計回り: currentAngle <= -targetAngle で停止
    return currentAngle > -targetAngle;
  } else {
    // 反時計回り: currentAngle >= targetAngle で停止
    return currentAngle < targetAngle;
  }
}

void IMURotation::setMotorControl()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  // setPowerを使用
  motorController.setLeftMotorPower(power * leftSign);
  motorController.setRightMotorPower(power * rightSign);
}

void IMURotation::updateMotorControl()
{
  // 最新の現在角度を取得
  float currentAngle = robot.getIMUControllerInstance().getAngle();

  // PID制御で操作量を計算（目標角度との偏差を補正）
  double pidOutput = pid.calculatePid(currentAngle, 0.01);  // 10ms周期

  // 基本パワーにPID出力を加算して動的制御
  double adjustedPower = power + pidOutput;

  // パワー値を-100~100の範囲に制限
  if(adjustedPower > 100.0) adjustedPower = 100.0;
  if(adjustedPower < -100.0) adjustedPower = -100.0;

  // モーターコントローラーにPID調整済みパワーを設定
  MotorController& motorController = robot.getMotorControllerInstance();
  motorController.setLeftMotorPower(adjustedPower * leftSign);
  motorController.setRightMotorPower(adjustedPower * rightSign);
}