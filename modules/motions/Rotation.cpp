/**
 * @file   Rotation.cpp
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#include "Rotation.h"

Rotation::Rotation(Robot& _robot, int _targetAngle, int _speed, bool _isClockwise)
  : Motion(_robot),
    targetAngle(_targetAngle),
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

  while(isMetContinuationCondition()) {
    // 回転速度（mm/秒）で指定しモーターを制御
    motorController.setLeftMotorSpeed(speed * leftSign);
    motorController.setRightMotorSpeed(speed * rightSign);

    clock.sleep(10000);  // 10000μs(10ms)程度のスリープでループを安定させる
  }

  // モーターを停止
  motorController.stopWheelsMotor();
  clock.sleep(10000);
}
