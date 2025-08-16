/**
 * @file   AngularVelocityTest.cpp
 * @brief  角速度PIDテスト用動作クラス
 * @author Hara1274
 */

#include "AngularVelocityTest.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

AngularVelocityTest::AngularVelocityTest(Robot& _robot)
  : Motion(_robot),
    angularVelocityPid(0.5, 0.01, 0.0, 0.0),
    testDuration(0)
{
}

AngularVelocityTest::AngularVelocityTest(Robot& _robot, double kp, double ki, double kd)
  : Motion(_robot),
    angularVelocityPid(kp, ki, kd, 0.0),
    testDuration(0)
{
}

void AngularVelocityTest::run()
{
  // IMU角度計算開始
  robot.getIMUControllerInstance().startAngleCalculation();
  
  std::cout << "角速度PIDテスト開始 - 目標角速度: " << TARGET_ANGULAR_VELOCITY << "deg/s (6秒間)" << std::endl;
  
  // 開始時刻を記録
  auto startTime = std::chrono::high_resolution_clock::now();
  
  // 10秒間テスト実行
  while(true) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    int elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    
    if(elapsedMs >= 6000) break;
    // 現在の角速度を取得
    double currentAngularVelocity = robot.getIMUControllerInstance().getCorrectedZAxisAngularVelocity();
    
    // 角速度PID制御で操作量を計算
    double motorPower = angularVelocityPid.calculatePid(TARGET_ANGULAR_VELOCITY - currentAngularVelocity, 0.01);
    
    // テスト結果をログ出力
    std::cout << "Time:" << elapsedMs << "ms, Target:" << TARGET_ANGULAR_VELOCITY 
              << ", Current:" << currentAngularVelocity 
              << ", Error:" << (TARGET_ANGULAR_VELOCITY - currentAngularVelocity)
              << ", Power:" << motorPower << std::endl;
    
    // モータに出力（時計回り回転）
    robot.getMotorControllerInstance().setLeftMotorPower(-motorPower);
    robot.getMotorControllerInstance().setRightMotorPower(motorPower);
    
    // 10ms待機
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  
  // テスト終了処理
  robot.getMotorControllerInstance().stopWheelsMotor();
  robot.getIMUControllerInstance().stopAngleCalculation();
  
  std::cout << "角速度PIDテスト終了" << std::endl;
}