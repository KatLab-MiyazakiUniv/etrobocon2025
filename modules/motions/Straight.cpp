/**
 * @file   Straight.cpp
 * @brief  直進動作の親クラス
 * @author HaruArima08
 */

#include "Straight.h"
#include <fstream>
#include <iomanip>

Straight::Straight(Robot& _robot, double _targetSpeed)
  : Motion(_robot), targetSpeed(_targetSpeed), speedCalculator(_robot, _targetSpeed)
{
}

void Straight::setSpeedPidGain(double rightKp, double rightKi, double rightKd, double leftKp,
                               double leftKi, double leftKd)
{
  speedCalculator.setSpeedPidGain(rightKp, rightKi, rightKd, leftKp, leftKi, leftKd);
}

// クラス内メンバにログファイルを持つのも可
std::ofstream logFile("speed_log.csv");

void Straight::run()
{
  if(!isMetPreCondition()) return;
  prepare();

  logFile << "Time,TargetSpeed,RightSpeed,LeftSpeed,RightPower,LeftPower\n";

  uint64_t startTime = robot.getClockInstance().now();

  while(isMetContinuationCondition()) {
    double rightPower = speedCalculator.calculateRightMotorPower();
    double leftPower = speedCalculator.calculateLeftMotorPower();

    robot.getMotorControllerInstance().setRightMotorPower(rightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(leftPower);

    double rightSpeed = robot.getMotorControllerInstance().getRightMotorSpeed();  // 仮関数
    double leftSpeed = robot.getMotorControllerInstance().getLeftMotorSpeed();    // 仮関数

    uint64_t nowTime = robot.getClockInstance().now();  // マイクロ秒単位
    double elapsed = (nowTime - startTime) / 1000000.0;

    logFile << std::fixed << std::setprecision(3) << elapsed << "," << targetSpeed << ","
            << rightSpeed << "," << leftSpeed << "," << rightPower << "," << leftPower << "\n";

    robot.getClockInstance().sleep(10000);
  }

  robot.getMotorControllerInstance().stopWheelsMotor();
  logFile.close();
}
// void Straight::run()
// {
//   // 事前条件判定が真でないときは終了する
//   if(!isMetPreCondition()) {
//     return;
//   }

//   // 事前準備
//   prepare();

//   // SpeedCalculator speedCalculator(robot,targetSpeed);

//   // 継続条件を満たしている間繰り返す
//   while(isMetContinuationCondition()) {
//     // Power値を計算
//     double currentRightPower = speedCalculator.calculateRightMotorPower();
//     double currentLeftPower = speedCalculator.calculateLeftMotorPower();

//     // モーターにPower値をセット
//     robot.getMotorControllerInstance().setRightMotorPower(currentRightPower);
//     robot.getMotorControllerInstance().setLeftMotorPower(currentLeftPower);

//     robot.getClockInstance().sleep(10000);  // 10000マイクロ秒(10ミリ秒)待機
//   }

//   // モータを停止
//   robot.getMotorControllerInstance().stopWheelsMotor();
// }
