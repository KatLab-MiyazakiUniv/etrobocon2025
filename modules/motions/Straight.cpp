/**
 * @file   Straight.cpp
 * @brief  直進動作の抽象クラス
 * @author bizyutyu
 */

#include "Straight.h"
using namespace std;

Straight::Straight(Robot& robot, double _targetSpeed)
  : Motion(robot), targetSpeed(_targetSpeed), initialDistance(0.0)
{
}
void Straight::run()
{
  // 事前条件判定が真でないときは終了する
  if(!isMetPreCondition()) {
    return;
  }
  // 呼び出し時の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // 継続条件を満たしている間繰り返す
  while(isMetContinuationCondition()) {
    // Power値を計算
    double currentLeftPower = robot.getMotorControllerInstance().getRightMotorPower();
    double currentRightPower = robot.getMotorControllerInstance().getLeftMotorPower();

    // モータにPower値をセット
    robot.getMotorControllerInstance().setLeftMotorPower(currentLeftPower);
    robot.getMotorControllerInstance().setRightMotorPower(currentRightPower);

    // 10ミリ秒待機
    robot.getClockInstance().sleep(10);
  }

  robot.getMotorControllerInstance().stopWheelsMotor();  // モータを停止
  robot.getClockInstance().sleep(10);
}
