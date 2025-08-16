/**
 * @file   AngularVelocityTest.h
 * @brief  角速度PIDテスト用動作クラス
 * @author Hara1274
 */

#ifndef ANGULAR_VELOCITY_TEST_H
#define ANGULAR_VELOCITY_TEST_H

#include "Motion.h"
#include "Pid.h"

class AngularVelocityTest : public Motion {
 public:
  AngularVelocityTest(Robot& _robot);
  AngularVelocityTest(Robot& _robot, double kp, double ki, double kd);

  /**
   * @brief 角速度PIDテストを実行する
   */
  void run() override;

 private:
  static constexpr double TARGET_ANGULAR_VELOCITY = 30.0;  // 目標角速度30deg/s
  Pid angularVelocityPid;  // 角速度PID制御クラス
  int testDuration;        // テスト継続時間(ms)
};

#endif