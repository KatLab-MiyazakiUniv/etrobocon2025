#include "IMURotation.h"
#include <cmath>
#include <iostream>

IMURotation::IMURotation(Robot& _robot, bool _isClockwise, float _targetAngle,
                         const PidGain& _pidGain)
  : Rotation(_robot, _isClockwise, _targetAngle, _pidGain)
{
}

void IMURotation::prepare()
{
  // 必要に応じて追加
}

bool IMURotation::isMetPreCondition()
{
  if(targetAngle <= 0.0f || targetAngle > 180.0f) {
    std::cerr << "IMURotation: targetAngle " << targetAngle << " is out of range." << std::endl;
    return false;
  }

  return true;
}

bool IMURotation::isMetContinuationCondition()
{
  // 誤差計算（±180範囲に正規化）
  float error = goalHeading - currentHeading;
  if(error > 180.0f) error -= 360.0f;
  if(error < -180.0f) error += 360.0f;
  return std::abs(error) > tolerance;
}