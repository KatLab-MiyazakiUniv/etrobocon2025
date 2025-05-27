/**
 * @file   Straight.cpp
 * @brief  直進動作の親クラス
 * @author HaruArima08
 */

#include "Straight.h"
using namespace std;

Straight::Straight(Robot& robot, double _targetSpeed) : Motion(robot), targetSpeed(_targetSpeed) {}
void Straight::run()
{
  // 事前条件判定が真でないときは終了する
  if(!isMetPreCondition()) {
    return;
  }
  // 呼び出し時の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // 速度に合うようにパワー値を設定
  robot.getMotorControllerInstance().setRightMotorSpeed(targetSpeed);
  robot.getMotorControllerInstance().setLeftMotorSpeed(targetSpeed);

  // 継続条件を満たしている間繰り返す
  while(isMetContinuationCondition()) {
    robot.getClockInstance().sleep(10000);  // 10000マイクロ秒(10ミリ秒)待機
  }

  // モータを停止
  robot.getMotorControllerInstance().stopWheelsMotor();
  robot.getClockInstance().sleep(10000);
}
