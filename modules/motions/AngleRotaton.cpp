/**
 * @file   AngleRotation.cpp
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#include "AngleRotation.h"

AngleRotation::AngleRotation(Robot& _robot, int _targetAngle, int _speed, bool _isClockwise)
  : Rotation(_robot, _targetAngle, _speed, _isClockwise),
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
  //   char buf[SMALL_BUF_SIZE];  // log用にメッセージを一時保持する領域

  // スピードが0以下の場合はwarningを出して終了する
  if(speed <= 0) {
    // snprintf(buf, SMALL_BUF_SIZE, "The speed value passed to AngleRotation is %d", speed);
    // logger.logWarning(buf);
    return false;
  }

  // targetAngleが0以下の場合はwarningを出して終了する
  if(targetAngle <= 0 || targetAngle >= 360) {
    // snprintf(buf, SMALL_BUF_SIZE, "The angle value passed to AngleRotation is %d", targetAngle);
    // logger.logWarning(buf);
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

void AngleRotation::logRunning()
{
  //   char buf[SMALL_BUF_SIZE];  // log用にメッセージを一時保持する領域
  //   const char* str = isClockwise ? "true" : "false";

  //   snprintf(buf, SMALL_BUF_SIZE, "Run AngleRotation (angle: %d, speed: %d, isClockwise: %s)",
  //            targetAngle, speed, str);
  //   logger.log(buf);
}