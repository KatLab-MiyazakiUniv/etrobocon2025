/**
 * @file   Straight.cpp
 * @brief  直進動作の親クラス
 * @author HaruArima08
 */

#include "Straight.h"
#include <thread>
#include <chrono>

Straight::Straight(Robot& _robot, double _targetSpeed) : Motion(_robot), targetSpeed(_targetSpeed)
{
}

void Straight::run()
{
  // 事前条件判定が真でないときは終了する
  if(!isMetPreCondition()) {
    return;
  }

  // 事前準備
  prepare();

  SpeedCalculator speedCalculator(robot, targetSpeed);

  // 継続条件を満たしている間繰り返す
  while(isMetContinuationCondition()) {
    // Power値を計算
    double currentRightPower = speedCalculator.calculateRightMotorPower();
    double currentLeftPower = speedCalculator.calculateLeftMotorPower();

    // モーターにPower値をセット
    robot.getMotorControllerInstance().setRightMotorPower(currentRightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(currentLeftPower);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10000マイクロ秒(10ミリ秒)待機
  }

  // モータを停止
  robot.getMotorControllerInstance().stopWheelsMotor();
}
