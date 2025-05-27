/**
 * @file   LineTrace.cpp
 * @brief  ライントレースの親クラス
 * @author miyahara046
 */

#include "LineTrace.h"
using namespace std;

LineTrace::LineTrace(Robot& _robot, double _targetSpeed, int _targetBrightness,
                     const PidGain& _pidGain, bool& _isLeftEdge)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetBrightness(_targetBrightness),
    pidGain(_pidGain),
    isLeftEdge(_isLeftEdge)

{
}

void LineTrace::run()
{
  Pid pid(pidGain.kp, pidGain.ki, pidGain.kd, targetBrightness);

  // 初期値を代入

  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());

  // 事前条件を判定する
  if(!isMetPreCondition()) {
    return;
  }

  // 呼び出し時の走行距離
  initLeftMileage
      = Mileage::calculateWheelMileage(robot.getMotorControllerInstance().getLeftMotorCount());
  initRightMileage
      = Mileage::calculateWheelMileage(robot.getMotorControllerInstance().getRightMotorCount());

  int logIntervalCount = 0;  // 走行ログを取得するタイミングを計るための変数

  // 左右で符号を変える
  int edgeSign = isLeftEdge ? -1 : 1;

  robot.getMotorControllerInstance().setRightMotorSpeed(720);
  robot.getMotorControllerInstance().setLeftMotorSpeed(720);

  // 継続条件を満たしている間ループ
  while(isMetContinuationCondition()) {
    // 初期Power値を計算
    double baseRightPower = robot.getMotorControllerInstance().getRightMotorPower();
    double baseLeftPower = robot.getMotorControllerInstance().getLeftMotorPower();

    // PIDで旋回値を計算
    double turningSpeed
        = pid.calculatePid(robot.getColorSensorInstance().getReflection()) * edgeSign;

    // モータのPower値をセット（前進の時0を下回らないように，後進の時0を上回らないようにセット）
    double rightPower = baseRightPower > 0.0 ? max(baseRightPower - turningSpeed, 0.0)
                                             : min(baseRightPower + turningSpeed, 0.0);
    double leftPower = baseLeftPower > 0.0 ? max(baseLeftPower + turningSpeed, 0.0)
                                           : min(baseLeftPower - turningSpeed, 0.0);
    robot.getMotorControllerInstance().setRightMotorPower(rightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(leftPower);

    // 10ms待機
    robot.getClockInstance().sleep(10000);
  }

  robot.getMotorControllerInstance().stopWheelsMotor();
}
