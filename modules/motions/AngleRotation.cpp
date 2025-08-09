/**
 * @file   AngleRotation.cpp
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#include "AngleRotation.h"

AngleRotation::AngleRotation(Robot& _robot, int _targetAngle, double _speed, bool _isClockwise)
  : Rotation(_robot, _speed, _isClockwise), targetAngle(_targetAngle)
{
}

void AngleRotation::prepare()
{
  // モーター走行距離ではなくIMUの角度を基準に設定
  float initialAngle = robot.getIMUControllerInstance().getAngle();
  targetAbsoluteAngle = normalizeAngle(initialAngle + (isClockwise ? -targetAngle : targetAngle));
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
  double currentAngle = robot.getIMUControllerInstance().getAngle();

  // 目標角度に達したか判定
  if(isClockwise) {
    // 時計回りは角度が減る（例: 350→340）
    // currentAngleがtargetAbsoluteAngle以下になったら終了
    if(currentAngle <= targetAbsoluteAngle) return false;
  } else {
    // 反時計回りは角度が増える（例: 10→20）
    // currentAngleがtargetAbsoluteAngle以上になったら終了
    if(currentAngle >= targetAbsoluteAngle) return false;
  }
  return true;
}