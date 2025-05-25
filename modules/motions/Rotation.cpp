/**
 * @file   Rotation.cpp
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#include "Rotation.h"

using namespace std;

Rotation::Rotation(Robot& _robot, int _targetAngle, int _power, bool _isClockwise)
  : Motion(_robot), targetAngle(_targetAngle), power(_power), isClockwise(_isClockwise)
{
}

void Rotation::run()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  if(!isMetPreCondition()) return;

  // 回転方向の符号：時計回り = +1, 反時計回り = -1
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;

  double initLeftMileage = Mileage::calculateWheelMileage(motorController.getLeftMotorCount());
  double initRightMileage = Mileage::calculateWheelMileage(motorController.getRightMotorCount());

  double targetDistance = M_PI * TREAD * targetAngle / 360;

  double targetLeftDistance = initLeftMileage + targetDistance * leftSign;
  double targetRightDistance = initRightMileage + targetDistance * rightSign;

  while(isMetContCondition(targetLeftDistance, targetRightDistance, leftSign, rightSign)) {
    // パワー値で左右のモーターを制御
    motorController.setLeftMotorPower(power * leftSign);
    motorController.setRightMotorPower(power * rightSign);

    // timer.sleep();  // 10ms程度のスリープでループを安定させる
  }

  // モーターを停止
  robot.getMotorControllerInstance().stopWheelsMotor();
  // timer.sleep(10);
}