/**
 * @file   AngleRotation.cpp
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#include "AngleRotation.h"

AngleRotation::AngleRotation(Robot& _robot, int _targetAngle, double _speed, bool _isClockwise)
  : Rotation(_robot, _isClockwise),
    targetAngle(_targetAngle),
    speed(_speed),
    targetLeftDistance(0.0),
    targetRightDistance(0.0)
{
}

void AngleRotation::prepare()
{
  MotorController& motorController = robot.getMotorControllerInstance();

  // 現在の走行距離を取得
  double initLeftMileage = Mileage::calculateWheelMileage(motorController.getLeftMotorCount());
  double initRightMileage = Mileage::calculateWheelMileage(motorController.getRightMotorCount());

  // 回頭距離 = π × TREAD(両輪間距離[mm]) × (角度 / 360) により各車輪の目標距離を算出
  double targetDistance = PI * TREAD * targetAngle / 360.0;

  // 目標走行距離を方向に応じて設定
  targetLeftDistance = initLeftMileage + targetDistance * leftSign;
  targetRightDistance = initRightMileage + targetDistance * rightSign;

  // モーター速度を設定
  motorController.setLeftMotorSpeed(speed * leftSign);
  motorController.setRightMotorSpeed(speed * rightSign);
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
  MotorController& motorController = robot.getMotorControllerInstance();
  // 残りの回転に必要な走行距離を算出
  double diffLeftDistance
      = (targetLeftDistance - Mileage::calculateWheelMileage(motorController.getLeftMotorCount()))
        * leftSign;
  double diffRightDistance
      = (targetRightDistance - Mileage::calculateWheelMileage(motorController.getRightMotorCount()))
        * rightSign;
  // 目標距離に到達した場合
  if(diffLeftDistance <= 0.0 && diffRightDistance <= 0.0) {
    return false;
  }
  return true;
}

void AngleRotation::updateMotorControl()
{
  // 固定制御のため何もしない
}