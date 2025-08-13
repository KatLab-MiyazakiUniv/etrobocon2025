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
    pid(_pidGain.kp, _pidGain.ki, _pidGain.kd, leftSign * _targetAngle)
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
  // 現在角度を１回だけ取得してメンバ変数に格納
  currentAngle = robot.getIMUControllerInstance().getAngle();

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
  // メンバ変数に格納された現在角度を使用
  // PID制御で操作量を計算（目標角度との偏差を補正）
  double correction = pid.calculatePid(currentAngle, 0.01);  // 10ms周期

  // PID出力を直接モーターパワーとして使用
  MotorController& motorController = robot.getMotorControllerInstance();
  motorController.setLeftMotorPower(leftSign * correction);
  motorController.setRightMotorPower(rightSign * correction);

  // 10ms待機して通信バッファオーバーフローを防ぐ
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}