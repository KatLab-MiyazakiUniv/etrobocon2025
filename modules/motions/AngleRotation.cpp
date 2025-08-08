/**
 * @file   AngleRotation.cpp
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#include "AngleRotation.h"

AngleRotation::AngleRotation(Robot& _robot, int _targetAngle, bool _isClockwise, const PidGain& _pidGain)
  : Rotation(_robot, _isClockwise, _targetAngle, _pidGain),
    lastTime(std::chrono::steady_clock::now())
{
}

void AngleRotation::prepare()
{
  // 角速度制御にするので距離は使わないが、必要なら初期値取得はしておく
  MotorController& motorController = robot.getMotorControllerInstance();

  // IMUの角速度で回すので距離目標は不要
  // 角度から回転にかかる時間を算出
  targetAngularVelocity = 720.0 * (rightSign); // 30 deg/sで回る（回転方向反映）

  double targetAngleRad = targetAngle * M_PI / 180.0;
  rotationTime = std::fabs(targetAngleRad / (targetAngularVelocity * DEG_TO_RAD));

  startTime = std::chrono::steady_clock::now();
}

bool AngleRotation::isMetPreCondition()
{
  // 角度が0以下または360以上なら終了
  if(targetAngle <= 0 || targetAngle >= 360) {
    std::cerr << "targetAngle=" << targetAngle << " は範囲外です。" << std::endl;
    return false;
  }

  return true;
}

bool AngleRotation::isMetContinuationCondition()
{
  auto now = std::chrono::steady_clock::now();
  double elapsed = std::chrono::duration<double>(now - startTime).count();
  return elapsed < rotationTime;
}
