/**
 * @file   Rotation.cpp
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#include "Rotation.h"

Rotation::Rotation(Robot& _robot, bool _isClockwise)
  : Motion(_robot),
    isClockwise(_isClockwise),
    leftSign(_isClockwise ? 1 : -1),
    rightSign(_isClockwise ? -1 : 1)
{
}

void Rotation::run()
{
  MotorController& motorController = robot.getMotorControllerInstance();
  spikeapi::Clock& clock = robot.getClockInstance();

  prepare();
  if(!isMetPreCondition()) return;

  while(isMetContinuationCondition()) {
    // 動的モーター制御
    updateMotorControl();
  }

  // モーターを停止
  motorController.stopWheelsMotor();
  motorController.setLeftMotorPower(0);
  motorController.setRightMotorPower(0);

}
