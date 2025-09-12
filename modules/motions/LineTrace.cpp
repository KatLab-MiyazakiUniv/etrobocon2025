/**
 * @file   LineTrace.cpp
 * @brief  ライントレースの親クラス
 * @author miyahara046
 */

#include "LineTrace.h"
#include <fstream>
#include <chrono>

extern std::ofstream globalLogFile;

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

  // ログファイルを空にする
  globalLogFile.close();
  globalLogFile.open("../../control_log.txt", std::ios::trunc);

  // 左右で符号を変える
  int edgeSign = robot.getIsLeftEdge() ? -1 : 1;

  SpeedCalculator speedCalculator(robot, targetSpeed);

  // 継続条件を満たしている間ループ
  while(isMetContinuationCondition()) {
    // Speed用PID計算開始ログ
    if(globalLogFile.is_open()) {
      globalLogFile << "[Speed-Right-Start]";
    }
    double baseRightPower = speedCalculator.calculateRightMotorPower();

    if(globalLogFile.is_open()) {
      globalLogFile << "[Speed-Left-Start]";
    }
    double baseLeftPower = speedCalculator.calculateLeftMotorPower();

    // ライントレース用PID計算開始ログ
    if(globalLogFile.is_open()) {
      globalLogFile << "[LineTrace-Start]";
    }

    // PIDで旋回値を計算
    double turningPower
        = pid.calculatePid(robot.getColorSensorInstance().getReflection()) * edgeSign;

    // モータのPower値をセット（前進の時0を下回らないように，後進の時0を上回らないようにセット）
    double rightPower = baseRightPower > 0.0 ? std::max(baseRightPower - turningPower, 0.0)
                                             : std::min(baseRightPower + turningPower, 0.0);
    double leftPower = baseLeftPower > 0.0 ? std::max(baseLeftPower + turningPower, 0.0)
                                           : std::min(baseLeftPower - turningPower, 0.0);

    // ライントレースログ出力
    if(globalLogFile.is_open()) {
      auto now = std::chrono::steady_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
      globalLogFile << "[LineTrace] 時刻=" << duration.count()
                    << " 反射=" << robot.getColorSensorInstance().getReflection()
                    << " 目標=" << targetBrightness << " 旋回=" << turningPower << "\n";
    }

    robot.getMotorControllerInstance().setRightMotorPower(rightPower);
    robot.getMotorControllerInstance().setLeftMotorPower(leftPower);

    // 10ms待機
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // モータを停止
  robot.getMotorControllerInstance().brakeWheelsMotor();
}
