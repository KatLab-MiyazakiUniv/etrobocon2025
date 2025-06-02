/**
 * @file   Rotation.cpp
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#include "Rotation.h"

Rotation::Rotation(Robot& _robot, int _targetAngle, int _speed, bool _isClockwise)
  : Motion(_robot), targetAngle(_targetAngle), speed(_speed), isClockwise(_isClockwise)
{
}

void Rotation::run()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  spikeapi::Clock& clock = robot.getClockInstance();
  if(!isMetPreCondition()) return;

  // 回転方向の符号：時計回り = +1, 反時計回り = -1
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;

  double initLeftMileage = Mileage::calculateWheelMileage(motorController.getLeftMotorCount());
  double initRightMileage = Mileage::calculateWheelMileage(motorController.getRightMotorCount());

  double targetDistance = M_PI * TREAD * targetAngle / 360;

  double targetLeftDistance = initLeftMileage + targetDistance * leftSign;
  double targetRightDistance = initRightMileage + targetDistance * rightSign;

  while(isMetContinuationCondition(targetLeftDistance, targetRightDistance, leftSign, rightSign)) {
    // 回転速度（°/秒）で指定しモーターを制御
    motorController.setLeftMotorSpeed(speed * leftSign);
    motorController.setRightMotorSpeed(speed * rightSign);

    clock.sleep(10000);  // 10000μs(10ms)程度のスリープでループを安定させる
  }

  // モーターを停止
  motorController.stopWheelsMotor();
  clock.sleep(10000);
}
