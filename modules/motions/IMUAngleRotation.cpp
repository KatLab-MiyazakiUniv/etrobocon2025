/**
 * @file   IMUAngleRotation.cpp
 * @brief  IMU角度指定回頭動作
 * @author Hara1274
 */

#include "IMUAngleRotation.h"

IMUAngleRotation::IMUAngleRotation(Robot& _robot, int _targetAngle, int _basePower,
                                   bool _isClockwise, const PidGain& _anglePidGain)
  : Rotation(_robot, _isClockwise),
    targetAngle(_targetAngle),
    basePower(_basePower),
    anglePid(_anglePidGain.kp, _anglePidGain.ki, _anglePidGain.kd, 0.0)
{
}

void IMUAngleRotation::prepare()
{
  // IMUの出力特性に合わせて目標角度を変換(IMUは時計回りをマイナス、反時計回りをプラスで出力)
  targetAngle = isClockwise ? -targetAngle : targetAngle;

  // IMU角度計算開始
  robot.getIMUControllerInstance().startAngleCalculation();
}

bool IMUAngleRotation::isMetPreCondition()
{
  // 角度をチェック
  if((targetAngle) <= 0 || (targetAngle) >= 360) {
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

bool IMUAngleRotation::isMetContinuationCondition()
{
  // 現在の角度を取得してメンバ変数に格納
  currentAngle = robot.getIMUControllerInstance().getAngle();

  // 角度誤差を計算
  angleError = targetAngle - currentAngle;

  // 誤差を±180範囲に正規化
  if(angleError > 180.0) angleError -= 360.0;
  if(angleError < -180.0) angleError += 360.0;

  // 誤差の絶対値が許容値より大きい間は継続
  bool shouldContinue = std::abs(angleError) > TOLERANCE;

  // 継続しない場合（終了する場合）はIMU角度計算を停止
  if(!shouldContinue) {
    robot.getIMUControllerInstance().stopAngleCalculation();
  }

  return shouldContinue;
}

void IMUAngleRotation::updateMotorControl()
{
  // PID制御により角度誤差から補正値を計算
  double pidCorrection = anglePid.calculatePid(angleError, 0.01);

  // 基準パワー値にPID補正を適用
  double motorPower = basePower + pidCorrection;

  // モータパワーを適用
  robot.getMotorControllerInstance().setLeftMotorPower(motorPower * leftSign);
  robot.getMotorControllerInstance().setRightMotorPower(motorPower * rightSign);

  // 10ms待機（これがないと通信バッファオーバーフローになる）
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}