/**
 * @file   LineTrace.cpp
 * @brief  ライントレースの親クラス
 * @author miyahara046
 */

#include "LineTrace.h"

LineTrace::LineTrace(Robot& _robot, double _targetSpeed, int _targetBrightness,
                     const PidGain& _pidGain)
  : Motion(_robot),
    targetSpeed(_targetSpeed),
    targetBrightness(_targetBrightness),
    pidGain(_pidGain)
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
  int edgeSign = robot.getIsLeftEdge() ? -1 : 1;

  SpeedCalculator speedCalculator(robot, targetSpeed);

  // 継続条件を満たしている間ループ
  while(isMetContinuationCondition()) {
    // 初期Speed値を計算
    double baseRightPower = speedCalculator.calculateRightMotorPower();
    double baseLeftPower = speedCalculator.calculateLeftMotorPower();

    // PIDで旋回値を計算
    double turningPower
        = pid.calculatePid(robot.getColorSensorInstance().getReflection()) * edgeSign;

    // モータのPower値をセット（前進の時0を下回らないように，後進の時0を上回らないようにセット）
    double rightPower = baseRightPower > 0.0 ? std::max(baseRightPower - turningPower, 0.0)
                                             : std::min(baseRightPower + turningPower, 0.0);
    double leftPower = baseLeftPower > 0.0 ? std::max(baseLeftPower + turningPower, 0.0)
                                           : std::min(baseLeftPower - turningPower, 0.0);
    robot.getMotorControllerInstance().setRightMotorPower(rightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(leftPower);

    // 10ms待機
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // モータを停止
  robot.getMotorControllerInstance().brakeWheelsMotor();
}
