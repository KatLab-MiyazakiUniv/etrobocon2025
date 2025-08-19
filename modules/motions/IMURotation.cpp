/**
 * @file   IMURotation.cpp
 * @brief  IMU角度指定回頭動作
 * @author Hara1274
 */

#include "IMURotation.h"

IMURotation::IMURotation(Robot& _robot, int _targetAngle, bool _isClockwise,
                         const PidGain& _anglePidGain)
  : Rotation(_robot, _isClockwise),
    targetAngle(_targetAngle),
    anglePid(_anglePidGain.kp, _anglePidGain.ki, _anglePidGain.kd, 0.0),
    angularVelocityPid(ANGULAR_VELOCITY_K_P, ANGULAR_VELOCITY_K_I, ANGULAR_VELOCITY_K_D, 0.0)
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
  // 絶対値で角度をチェック（変換後の値は負になる可能性があるため）
  if(std::abs(targetAngle) <= 0 || std::abs(targetAngle) >= 360) {
    std::cerr << "targetAngle=" << targetAngle << " は範囲外です。" << std::endl;
    return false;
  }

  // IMU角度計算が既に開始されている場合は開始できない
  if(robot.getIMUControllerInstance().isAngleCalculating()) {
    std::cerr << "IMU角度計算が既に開始されています。" << std::endl;
    return false;
  }

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

void IMURotation::updateMotorControl()
{
  // 目標角度に達するまでの残りの角度を計算
  double angleError = targetAngle - currentAngle;

  // 誤差を±180範囲に正規化
  if(angleError > 180.0) angleError -= 360.0;
  if(angleError < -180.0) angleError += 360.0;

  // PID制御により角度から目標角速度を決定
  double targetAngularVelocity = anglePid.calculatePid(angleError, 0.01);

  // PID制御により目標角速度からモータパワーを決定
  double motorPower = angularVelocityPid.calculatePid(
      targetAngularVelocity - robot.getIMUControllerInstance().getCorrectedZAxisAngularVelocity(),
      0.01);

  // モータパワーを適用（IMUの出力特性に合わせてモータパワーの符号を反転）
  robot.getMotorControllerInstance().setLeftMotorPower(-motorPower * leftSign);
  robot.getMotorControllerInstance().setRightMotorPower(-motorPower * rightSign);

  // 10ms待機（これがないと通信バッファオーバーフローになる）
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}