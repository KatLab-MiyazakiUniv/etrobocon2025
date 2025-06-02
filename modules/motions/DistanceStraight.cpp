/**
 * @file   DistanceStraight.cpp
 * @brief  目標距離まで直進するクラス
 * @author HaruArima08
 */

#include "DistanceStraight.h"
using namespace std;

DistanceStraight::DistanceStraight(Robot& robot, double _targetDistance, double _targetSpeed)
  : Straight(robot, _targetSpeed), targetDistance(_targetDistance)
{
}

// 目標距離まで直進する際の事前条件
bool DistanceStraight::isMetPreCondition()
{
  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    std::cout << "[Warning] targetSpeed is 0.0\n";
    return false;
  }

  // targetDistance値が0以下の場合はwarningを出して終了する
  if(targetDistance <= 0.0) {
    std::cout << "[Warning] targetDistance is less than or equal to 0.0\n";
    return false;
  }

  return true;
}

void DistanceStraight::prepare()
{
  // 呼び出し時の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);
}

bool DistanceStraight::isMetContinuationCondition()
{
  // 現在の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  // 現在の走行距離が目標走行距離に達した場合falseを返す
  // ここでは、左右どちらとも達したかどうかで判断する
  if((fabs(currentDistance - initialDistance) >= targetDistance)) {
    return false;
  }

  // 現在の走行距離が目標走行距離に達していなければtrueを返す
  return true;
}
