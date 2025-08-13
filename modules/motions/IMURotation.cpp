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
    pid(_pidGain.kp, _pidGain.ki, _pidGain.kd, leftSign * _targetAngle),
    tolerance(1.0)
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
  // 現在の角度を取得してメンバ変数に格納
  currentAngle = robot.getIMUControllerInstance().getAngle();

  // 目標角度（leftSign * targetAngle）との誤差を計算
  float goalAngle = leftSign * targetAngle;
  float error = goalAngle - currentAngle;

  // 誤差計算（±180範囲に正規化）
  if(error > 180.0f) error -= 360.0f;
  if(error < -180.0f) error += 360.0f;

  // 誤差の絶対値が許容値より大きい間は継続
  return std::abs(error) > tolerance;
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
  // メンバ変数に格納された現在角度を使用
  // PID制御で操作量を計算（目標角度との偏差を補正）
  double correction = pid.calculatePid(currentAngle, 0.01);

  // 基本パワー + PID補正値
  double leftPower = power * leftSign + correction;
  double rightPower = power * rightSign - correction;

  // パワー値を-100~100の範囲に制限
  if(leftPower > 100.0) leftPower = 100.0;
  if(leftPower < -100.0) leftPower = -100.0;
  if(rightPower > 100.0) rightPower = 100.0;
  if(rightPower < -100.0) rightPower = -100.0;

  MotorController& motorController = robot.getMotorControllerInstance();
  motorController.setLeftMotorPower(leftPower);
  motorController.setRightMotorPower(rightPower);

  // 10ms待機（これがないと通信バッファオーバーフローになる）
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}