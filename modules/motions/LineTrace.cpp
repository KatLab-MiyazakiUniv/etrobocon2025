/**
 * @file   LineTrace.cpp
 * @brief  ライントレースの親クラス
 * @author miyahara046
 */

#include "LineTrace.h"
using namespace std;

LineTrace::LineTrace(Robot& _robot, double _targetSpeed, int _targetBrightness,
                     const PidGain& _pidGain, bool& _isLeftEdge)
  :  // colorSensor(EPort::PORT_A),
    Motion(robot),
    robot(_robot),
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

  initDistance = Mileage::calculateMileage(motorController.getRightMotorCount(),
                                           motorController.getLeftMotorCount());

  // 事前条件を判定する
  if(!isMetPreCondition(targetSpeed)) {
    return;
  }

  // 左右で符号を変える
  int edgeSign = isLeftEdge ? -1 : 1;

  // 呼び出し時の走行距離
  initLeftMileage = Mileage::calculateWheelMileage(motorController.getLeftMotorCount());
  initRightMileage = Mileage::calculateWheelMileage(motorController.getRightMotorCount());

  int logIntervalCount = 0;  // 走行ログを取得するタイミングを計るための変数

  // 継続条件を満たしている間ループ
  while(isMetContinuationCondition()) {
    // 初期pwm値を計算
    double baseRightPwm = motorController.getRightMotorPower();
    double baseLeftPwm = motorController.getLeftMotorPower();

    // PIDで旋回値を計算
    /*double turningPwm = pid.calculatePid(colorSensor.getReflection()) * edgeSign;

    // モータのPWM値をセット（前進の時0を下回らないように，後進の時0を上回らないようにセット）
    double rightPwm = baseRightPwm > 0.0 ? max(baseRightPwm - turningPwm, 0.0)
                                         : min(baseRightPwm + turningPwm, 0.0);
    double leftPwm = baseLeftPwm > 0.0 ? max(baseLeftPwm + turningPwm, 0.0)
                                       : min(baseLeftPwm - turningPwm, 0.0);
    motorController.setRightMotorPower(rightPwm);
    motorController.setLeftMotorPower(leftPwm);

    //  10ループに1回走行ログを取得
    if(logIntervalCount % 10 == 0) {
      現在の明るさを取得
      double currentReflection = colorSensor.getReflection();

      現在のRGB値を取得
      spikeapi::ColorSensor::RGB currentRgb;
      colorSensor.getRGB(currentRgb);
    }
    logIntervalCount++;
    */
    // 10ms待機
    clock.sleep(10);
  }

  motorController.stopWheelsMotor();
}
