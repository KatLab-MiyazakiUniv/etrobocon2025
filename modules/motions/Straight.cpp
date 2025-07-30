/**
 * @file   Straight.cpp
 * @brief  直進動作の親クラス
 * @author HaruArima08
 */

#include "Straight.h"

Straight::Straight(Robot& _robot, double _targetSpeed, double _rightKp, double _rightKi,
                   double _rightKd, double _leftKp, double _leftKi, double _leftKd)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    rightKp(_rightKp),
    rightKi(_rightKi),
    rightKd(_rightKd),
    leftKp(_leftKp),
    leftKi(_leftKi),
    leftKd(_leftKd),
    speedCalculator(_robot, _targetSpeed)
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
  speedCalculator.setSpeedPidGain(rightKp, rightKi, rightKd, leftKp, leftKi, leftKd);

  std::cout << rightKp<<" , "<<leftKp<<std::endl;

  // 継続条件を満たしている間繰り返す
  while(isMetContinuationCondition()) {
    // Power値を計算
    double currentRightPower = speedCalculator.calculateRightMotorPower();
    double currentLeftPower = speedCalculator.calculateLeftMotorPower();

    //モーターにPower値をセット
    robot.getMotorControllerInstance().setRightMotorPower(currentRightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(currentLeftPower);


    double right = robot.getMotorControllerInstance().getRightMotorSpeed();
    double left = robot.getMotorControllerInstance().getLeftMotorSpeed();
  
  
    std::cout << left << "  |  " << right << std::endl;
    std::cout << currentLeftPower << "  |  " << currentRightPower << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10000マイクロ秒(10ミリ秒)待機
  }

  // モータを停止
  robot.getMotorControllerInstance().stopWheelsMotor();
}
