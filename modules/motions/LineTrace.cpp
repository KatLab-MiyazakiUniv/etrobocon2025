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

  // 事前条件を判定する
  if(!isMetPreCondition()) {
    return;
  }

  // 事前準備
  prepare();

  // 左右で符号を変える
  int edgeSign = isLeftEdge ? -1 : 1;

  // 初期Speed値の設定
  robot.getMotorControllerInstance().setRightMotorSpeed(FIRST_SPEED);
  robot.getMotorControllerInstance().setLeftMotorSpeed(FIRST_SPEED);

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
