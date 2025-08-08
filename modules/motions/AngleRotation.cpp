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
  targetAngularVelocity = 30.0 * (leftSign); // 30 deg/sで回る（回転方向反映）

  double targetAngleRad = targetAngle * M_PI / 180.0;
  rotationTime = std::fabs(targetAngleRad / (targetAngularVelocity * M_PI / 180.0));

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
    MotorController& motorController = robot.getMotorControllerInstance();

    // 許容誤差 [mm]（必要に応じて調整）
    constexpr double DISTANCE_TOLERANCE = 1.0; 

    double currentLeftMileage  = Mileage::calculateWheelMileage(motorController.getLeftMotorCount());
    double currentRightMileage = Mileage::calculateWheelMileage(motorController.getRightMotorCount());

    double diffLeftDistance = (targetLeftDistance - currentLeftMileage) * leftSign;
    double diffRightDistance = (targetRightDistance - currentRightMileage) * rightSign;

    // 走った距離を計算（現在距離 - 初期距離）
    double traveledDistanceLeft = currentLeftMileage - initLeftMileage;
    double traveledDistanceRight = currentRightMileage - initRightMileage;

    // 左右の距離差から回転距離を計算
    double traveledDistance = (traveledDistanceLeft - traveledDistanceRight) / 2.0;
    double currentAngle = traveledDistance / (PI * TREAD) * 360.0;

    double angleError = targetAngle - currentAngle;

    std::cout << "Current Angle: " << currentAngle << " deg, "
              << "Angle Error: " << angleError << " deg" << std::endl;

    if(std::fabs(diffLeftDistance) <= DISTANCE_TOLERANCE &&
       std::fabs(diffRightDistance) <= DISTANCE_TOLERANCE) {
        return false;
    }
    return true;
}
