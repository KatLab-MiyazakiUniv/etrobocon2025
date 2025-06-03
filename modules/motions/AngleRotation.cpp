/**
 * @file   AngleRotation.cpp
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#include "AngleRotation.h"

AngleRotation::AngleRotation(Robot& robot, int targetAngle, int speed, bool isClockwise)
  : Rotation(robot, targetAngle, speed, isClockwise),
    targetLeftDistance(0.0),
    targetRightDistance(0.0)
{
}

void AngleRotation::prepare()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  double initLeftMileage = Mileage::calculateWheelMileage(motorController.getLeftMotorCount());
  double initRightMileage = Mileage::calculateWheelMileage(motorController.getRightMotorCount());
  double targetDistance = M_PI * TREAD * targetAngle / 360;
  targetLeftDistance = initLeftMileage + targetDistance * leftSign;
  targetRightDistance = initRightMileage + targetDistance * rightSign;
}

bool AngleRotation::isMetPreCondition()
{
  // スピードが0以下なら終了
  if(speed <= 0) {
    std::cerr << "speed=" << speed << " は無効な値です。\n";
    return false;
  }

  // 角度が0以下または360以上なら終了
  if(targetAngle <= 0 || targetAngle >= 360) {
    std::cerr << "targetAngle=" << targetAngle << " は範囲外です。\n";
    return false;
  }

  return true;
}

bool AngleRotation::isMetContinuationCondition()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  // 残りの回転に必要な走行距離を算出
  double diffLeftDistance
      = (targetLeftDistance - Mileage::calculateWheelMileage(motorController.getLeftMotorCount()))
        * leftSign;
  double diffRightDistance
      = (targetRightDistance - Mileage::calculateWheelMileage(motorController.getRightMotorCount()))
        * rightSign;
  // 目標距離に到達した場合
  if(diffLeftDistance <= 0 && diffRightDistance <= 0) {
    return false;
  }
  return true;
}