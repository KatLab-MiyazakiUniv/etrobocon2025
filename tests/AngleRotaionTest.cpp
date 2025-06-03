/**
 * @file   PwmRotationTest.cpp
 * @brief  PwmRotationクラスのテスト
 * @author takahashitom
 */

#include "AngleRotation.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include "Robot.h"
#include "SystemInfo.h"

using namespace std;

namespace etrobocon2024_test {

  // @see https://github.com/KatLab-MiyazakiUniv/etrobocon2022/blob/main/docs/odometry.md
  constexpr double TRANSFORM = 2.0 * WHEEL_RADIUS / TREAD;  // 回頭角度を求める式の係数

  // 右回頭のテスト
  TEST(AngleRotationTest, runRight)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();

    int angle = 90;
    int speed = 300;
    bool isClockwise = true;

    AngleRotation AngleRotation(robot, angle, speed, isClockwise);

    double expected = angle;  // 指定した回頭角度を期待値とする

    // 一回のsetMotorSpeed()でダミーのモーターに加算される値はspeed * 0.05
    double error = speed * 0.05 * TRANSFORM;

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    AngleRotation.run();  // 右回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_LE(expected, actual);
    EXPECT_GE(expected + error, actual);
  }

  //   左回頭のテスト
  TEST(AngleRotationTest, runLeft)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();
    motorController.resetWheelsMotorPower();

    int angle = 180;
    int speed = 300;
    bool isClockwise = false;

    AngleRotation AngleRotation(robot, angle, speed, isClockwise);

    double expected = angle;  // 指定した回頭角度を期待値とする

    // 一回のsetMotorSpeed()でダミーのモーターに加算される値はspeed * 0.05
    double error = speed * 0.05 * TRANSFORM;

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    AngleRotation.run();  // 左回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_LE(expected, actual);
    EXPECT_GE(expected + error, actual);
  }

  //   // PWM値を0に設定して回頭するテスト
  //   TEST(PwmRotationTest, runZeroPWM)
  //   {
  //     Controller controller;
  //     controller.resetRightMotorPwm();
  //     controller.resetLeftMotorPwm();

  //     Measurer measurer;
  //     int angle = 45;
  //     int pwm = 0;
  //     bool isClockwise = true;
  //     PwmRotation PwmRotation(angle, pwm, isClockwise);

  //     double expected = 0;  // 回頭しない

  //     // Warning文
  //     string expectedOutput = "\x1b[36m";  // 文字色をシアンに
  //     expectedOutput += "Warning: The pwm value passed to PwmRotation is 0";
  //     expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

  //     // 回頭前のモータカウント
  //     int initialRightMotorCount = measurer.getRightCount();
  //     int initialLeftMotorCount = measurer.getLeftCount();

  //     testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
  //     PwmRotation.run();                                             // 右回頭を実行
  //     string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

  //     // 回頭後に各モータが回転した角度
  //     int rightMotorCount = abs(measurer.getRightCount() - initialRightMotorCount);
  //     int leftMotorCount = abs(measurer.getLeftCount() - initialLeftMotorCount);
  //     double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

  //     EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
  //     EXPECT_EQ(expected, actual);              // 回頭の前後で角度に変化はない
  //   }

  //   // PWM値をマイナスに設定して回頭するテスト
  //   TEST(PwmRotationTest, runMinusPWM)
  //   {
  //     Controller controller;
  //     controller.resetRightMotorPwm();
  //     controller.resetLeftMotorPwm();

  //     Measurer measurer;
  //     int angle = 45;
  //     int pwm = -100;
  //     bool isClockwise = true;
  //     PwmRotation PwmRotation(angle, pwm, isClockwise);

  //     double expected = 0;  // 回頭しない

  //     // Warning文
  //     string expectedOutput = "\x1b[36m";  // 文字色をシアンに
  //     expectedOutput += "Warning: The pwm value passed to PwmRotation is " + to_string(pwm);
  //     expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

  //     // 回頭前のモータカウント
  //     int initialRightMotorCount = measurer.getRightCount();
  //     int initialLeftMotorCount = measurer.getLeftCount();

  //     testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
  //     PwmRotation.run();                                             // 右回頭を実行
  //     string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

  //     // 回頭後に各モータが回転した角度
  //     int rightMotorCount = abs(measurer.getRightCount() - initialRightMotorCount);
  //     int leftMotorCount = abs(measurer.getLeftCount() - initialLeftMotorCount);
  //     double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

  //     EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
  //     EXPECT_EQ(expected, actual);              // 回頭の前後で角度に変化はない
  //   }

  //   // 回頭角度を0に設定して回頭するテスト
  //   TEST(PwmRotationTest, runZeroAngle)
  //   {
  //     Controller controller;
  //     controller.resetRightMotorPwm();
  //     controller.resetLeftMotorPwm();

  //     Measurer measurer;
  //     int angle = 0;
  //     int pwm = 100;
  //     bool isClockwise = true;
  //     PwmRotation PwmRotation(angle, pwm, isClockwise);

  //     double expected = 0;  // 回頭しない

  //     // Warning文
  //     string expectedOutput = "\x1b[36m";  // 文字色をシアンに
  //     expectedOutput += "Warning: The angle value passed to PwmRotation is 0";
  //     expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

  //     // 回頭前のモータカウント
  //     int initialRightMotorCount = measurer.getRightCount();
  //     int initialLeftMotorCount = measurer.getLeftCount();

  //     testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
  //     PwmRotation.run();                                             // 右回頭を実行
  //     string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

  //     // 回頭後に各モータが回転した角度
  //     int rightMotorCount = abs(measurer.getRightCount() - initialRightMotorCount);
  //     int leftMotorCount = abs(measurer.getLeftCount() - initialLeftMotorCount);
  //     double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

  //     EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
  //     EXPECT_EQ(expected, actual);              // 回頭の前後で角度に変化はない
  //   }

  //   // 回頭角度をマイナスに設定して回頭するテスト
  //   TEST(PwmRotationTest, runMinusAngle)
  //   {
  //     Controller controller;
  //     controller.resetRightMotorPwm();
  //     controller.resetLeftMotorPwm();

  //     Measurer measurer;
  //     int angle = -180;
  //     int pwm = 100;
  //     bool isClockwise = true;
  //     PwmRotation PwmRotation(angle, pwm, isClockwise);

  //     double expected = 0;  // 回頭しない

  //     // Warning文
  //     string expectedOutput = "\x1b[36m";  // 文字色をシアンに
  //     expectedOutput += "Warning: The angle value passed to PwmRotation is " + to_string(angle);
  //     expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

  //     // 回頭前のモータカウント
  //     int initialRightMotorCount = measurer.getRightCount();
  //     int initialLeftMotorCount = measurer.getLeftCount();

  //     testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
  //     PwmRotation.run();                                             // 右回頭を実行
  //     string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

  //     // 回頭後に各モータが回転した角度
  //     int rightMotorCount = abs(measurer.getRightCount() - initialRightMotorCount);
  //     int leftMotorCount = abs(measurer.getLeftCount() - initialLeftMotorCount);
  //     double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

  //     EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
  //     EXPECT_EQ(expected, actual);              // 回頭の前後で角度に変化はない
  //   }

  //   // 回頭角度を360度以上に設定して回頭するテスト
  //   TEST(PwmRotationTest, runOverAngle)
  //   {
  //     Controller controller;
  //     controller.resetRightMotorPwm();
  //     controller.resetLeftMotorPwm();

  //     Measurer measurer;
  //     int angle = 360;
  //     int pwm = 100;
  //     bool isClockwise = true;
  //     PwmRotation PwmRotation(angle, pwm, isClockwise);

  //     double expected = 0;  // 回頭しない

  //     // Warning文
  //     string expectedOutput = "\x1b[36m";  // 文字色をシアンに
  //     expectedOutput += "Warning: The angle value passed to PwmRotation is " + to_string(angle);
  //     expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

  //     // 回頭前のモータカウント
  //     int initialRightMotorCount = measurer.getRightCount();
  //     int initialLeftMotorCount = measurer.getLeftCount();

  //     testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
  //     PwmRotation.run();                                             // 右回頭を実行
  //     string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

  //     // 回頭後に各モータが回転した角度
  //     int rightMotorCount = abs(measurer.getRightCount() - initialRightMotorCount);
  //     int leftMotorCount = abs(measurer.getLeftCount() - initialLeftMotorCount);
  //     double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

  //     EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
  //     EXPECT_EQ(expected, actual);              // 回頭の前後で角度に変化はない
  //   }
}  // namespace etrobocon2024_test