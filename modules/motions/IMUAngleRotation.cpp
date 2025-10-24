/**
 * @file   IMUAngleRotation.cpp
 * @brief  IMU角度指定回頭動作
 * @author Hara1274
 */

#include "IMUAngleRotation.h"

IMUAngleRotation::IMUAngleRotation(Robot& _robot, int _targetAngle, int _basePower,
                                   bool _isClockwise, const PidGain& _anglePidGain,
                                   bool _isAbsoluteAngleMode)
  : Rotation(_robot, _isClockwise),
    targetAngle(_targetAngle),
    basePower(_basePower),
    anglePid(_anglePidGain.kp, _anglePidGain.ki, _anglePidGain.kd, 0.0),
    isAbsoluteAngleMode(_isAbsoluteAngleMode),
    initialAngle(0.0),
    totalAngleToTurn(0.0)
{
}

void IMUAngleRotation::prepare()
{
  // 相対角度モードで、かつIMUの角度計算が開始されていなければ、この動作で計算を開始する
  if(!isAbsoluteAngleMode && !robot.getIMUControllerInstance().shouldContinueCalculation()) {
    robot.getIMUControllerInstance().startAngleCalculation();
  }

  // 開始時の角度を取得
  initialAngle = robot.getIMUControllerInstance().getAngle();

  if(!isAbsoluteAngleMode) {
    // 相対角度モード: targetAngleを相対角度として扱う
    if(isClockwise) {
      // 時計回りの場合、targetAngleをそのまま総回頭角度
      totalAngleToTurn = targetAngle;
    } else {
      // 反時計回りの場合、targetAngleを負の値にして総回頭角度に設定
      totalAngleToTurn = -targetAngle;
    }
  } else {
    // 絶対角度モード: targetAngleを絶対角度として扱う
    // 開始角度から目標角度への差を、剰余演算で0～360°に正規化し、時計回りの角度を算出
    double clockwiseAngle = fmod(targetAngle - initialAngle + 360.0, 360.0);

    if(isClockwise) {
      // 時計回りに回頭する場合、clockwiseAngleをそのまま総回頭角度
      totalAngleToTurn = clockwiseAngle;
    } else {
      // 反時計回りの場合、clockwiseAngleが小さな値のときは総回頭角度を0にし、360度の回転を防止
      if(clockwiseAngle < 0.01) {
        totalAngleToTurn = 0;
      } else {
        // それ以外は、時計回り角度から360を引いて総回頭角度に変換
        totalAngleToTurn = clockwiseAngle - 360.0;
      }
    }
  }
}

bool IMUAngleRotation::isMetPreCondition()
{
  // 角度をチェック
  if((targetAngle) < 0 || (targetAngle) >= 360) {
    std::cerr << "targetAngle=" << targetAngle << " は範囲外です。" << std::endl;
    return false;
  }

  // 絶対角度モードで、かつIMUの角度計算が(ISコマンドで)開始されていなければエラー
  if(isAbsoluteAngleMode && !robot.getIMUControllerInstance().shouldContinueCalculation()) {
    std::cerr << "絶対角度モードではIS,startで角度計算を事前に開始する必要があります。"
              << std::endl;
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

bool IMUAngleRotation::isMetContinuationCondition()
{
  // 現在の角度を取得してメンバ変数に格納
  currentAngle = robot.getIMUControllerInstance().getAngle();

  // 開始角度からどれだけ回転したかを計算
  double angleTurned = currentAngle - initialAngle;

  // 360度の境界をまたいだ場合の補正
  if(isClockwise && angleTurned < 0.0) {
    // 時計回りで差が負になったら、周回したとみなして360度足す
    angleTurned += 360.0;
  } else if(!isClockwise && angleTurned > 0.0) {
    // 反時計回りで差が正になったら、周回したとみなして360度引く
    angleTurned -= 360.0;
  }

  // 残りの回転角度を誤差として設定
  angleError = totalAngleToTurn - angleTurned;

  // 誤差の絶対値が許容値より大きい間は継続
  bool shouldContinue = std::abs(angleError) > TOLERANCE;

  // 継続しない場合（終了する場合）
  if(!shouldContinue) {
    // この動作で角度計算を開始した場合のみ、計算を停止
    if(!robot.getIMUControllerInstance().shouldContinueCalculation()) {
      robot.getIMUControllerInstance().stopAngleCalculation();
    }
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