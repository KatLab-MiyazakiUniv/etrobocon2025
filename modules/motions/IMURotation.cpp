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
    pid(_pidGain.kp, _pidGain.ki, _pidGain.kd, 0.0)
{
}

void IMURotation::prepare()
{
  // IMUの出力特性に合わせて目標角度を変換(IMUは時計回りをマイナス、反時計回りをプラスで出力)
  targetAngle = isClockwise ? -targetAngle : targetAngle;

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

  float error = targetAngle - currentAngle;

  // 誤差計算（±180範囲に正規化）
  if(error > 180.0f) error -= 360.0f;
  if(error < -180.0f) error += 360.0f;

  // 誤差の絶対値が許容値より大きい間は継続
  bool shouldContinue = std::abs(error) > TOLERANCE;

  // 継続しない場合（終了する場合）はIMU角度計算を停止
  if(!shouldContinue) {
    robot.getIMUControllerInstance().stopAngleCalculation();
  }

  return shouldContinue;
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
  // 目標角度との偏差を計算
  double error = targetAngle - currentAngle;

  // 誤差を±180範囲に正規化
  if(error > 180.0) error -= 360.0;
  if(error < -180.0) error += 360.0;

  // PID制御で操作量を計算（偏差を入力として使用）
  double correction = pid.calculatePid(error, 0.01);

  // 基本パワー + PID補正値
  double leftPower = power * leftSign + correction;
  double rightPower = power * rightSign - correction;

  MotorController& motorController = robot.getMotorControllerInstance();
  motorController.setLeftMotorPower(leftPower);
  motorController.setRightMotorPower(rightPower);

  // 10ms待機（これがないと通信バッファオーバーフローになる）
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}