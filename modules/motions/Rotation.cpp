/**
 * @file   Rotation.cpp
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#include "Rotation.h"

Rotation::Rotation(Robot& _robot, double _speed, bool _isClockwise)
  : Motion(_robot),
    speed(_speed),
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

  // 回転速度（mm/秒）で指定しモーターを制御
  motorController.setLeftMotorSpeed(speed * leftSign);
  motorController.setRightMotorSpeed(speed * rightSign);

  while(isMetContinuationCondition()) {
    // 回頭継続条件が満たされるまで待機（モーターが走行中）
  }

  // モーターを停止
  motorController.stopWheelsMotor();
}
