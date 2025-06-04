/**
 * @file   AngleRotationTest.cpp
 * @brief  AngleRotationクラスのテスト
 * @author Hara1274
 */

#include "AngleRotation.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include "Robot.h"
#include "SystemInfo.h"

namespace etrobocon2025_test {

  // @see https://github.com/KatLab-MiyazakiUniv/etrobocon2022/blob/main/docs/odometry.md
  constexpr double TRANSFORM = 2.0 * WHEEL_RADIUS / TREAD;  // 回頭角度を求める式の係数

  // 右回頭のテスト
  TEST(AngleRotationTest, RunRight)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();

    int angle = 90;
    double speed = 300.0;
    bool isClockwise = true;

    AngleRotation angleRotation(robot, speed, isClockwise, angle);

    double expected = angle;  // 指定した回頭角度を期待値とする

    // 実機の setSpeed() 後のモータカウントの増減を模倣するため、ダミークラスで
    // isSetSpeedがtrueのとき、getCount()を呼ぶ度に count += speed× 0.05される。
    // よって、1ループあたりの余分な回転角は speed × 0.05 × TRANSFORM を誤差として見込む。
    double error = speed * 0.05 * TRANSFORM;

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    angleRotation.run();  // 右回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_LE(expected, actual);
    EXPECT_GE(expected + error, actual);
  }

  // 左回頭のテスト
  TEST(AngleRotationTest, RunLeft)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();
    motorController.resetWheelsMotorPower();

    int angle = 180;
    double speed = 300.0;
    bool isClockwise = false;

    AngleRotation angleRotation(robot, speed, isClockwise, angle);

    double expected = angle;  // 指定した回頭角度を期待値とする

    // 実機の setSpeed() 後のモータカウントの増減を模倣するため、ダミークラスで
    // isSetSpeedがtrueのとき、getCount()を呼ぶ度に count += speed× 0.05される。
    // よって、1ループあたりの余分な回転角は speed × 0.05 × TRANSFORM を誤差として見込む。
    double error = speed * 0.05 * TRANSFORM;

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    angleRotation.run();  // 左回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_LE(expected, actual);
    EXPECT_GE(expected + error, actual);
  }

  // speedを0に設定したときに回頭をせずに終了するかのテスト
  TEST(AngleRotationTest, RunZeroSpeed)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();
    motorController.resetWheelsMotorPower();

    int angle = 45;
    double speed = 0.0;
    bool isClockwise = true;

    AngleRotation angleRotation(robot, speed, isClockwise, angle);

    double expected = 0.0;  // 回頭しない

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    angleRotation.run();  // 回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_EQ(expected, actual);
  }

  // speedをマイナスに設定したときに回頭をせずに終了するかのテスト
  TEST(AngleRotationTest, RunMinusSpeed)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();
    motorController.resetWheelsMotorPower();

    int angle = 45;
    double speed = -0.01;
    bool isClockwise = true;

    AngleRotation angleRotation(robot, speed, isClockwise, angle);

    double expected = 0.0;  // 回頭しない

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    angleRotation.run();  // 回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_EQ(expected, actual);
  }

  // 回頭角度を0に設定したときに回頭をせずに終了するかのテスト
  TEST(AngleRotationTest, RunZeroAngle)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();
    motorController.resetWheelsMotorPower();

    int angle = 0;
    double speed = 300.0;
    bool isClockwise = true;

    AngleRotation angleRotation(robot, speed, isClockwise, angle);

    double expected = 0.0;  // 回頭しない

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    angleRotation.run();  // 回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_EQ(expected, actual);
  }

  // 回頭角度をマイナスに設定したときに回頭をせずに終了するかのテスト
  TEST(AngleRotationTest, RunMinusAngle)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();
    motorController.resetWheelsMotorPower();

    int angle = -1;
    double speed = 300.0;
    bool isClockwise = true;

    AngleRotation angleRotation(robot, speed, isClockwise, angle);

    double expected = 0.0;  // 回頭しない

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    angleRotation.run();  // 回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_EQ(expected, actual);
  }

  // 回頭角度を360度以上に設定したときに回頭をせずに終了するかのテスト
  TEST(AngleRotationTest, RunOverAngle)
  {
    Robot robot;
    MotorController& motorController = robot.getMotorControllerInstance();
    motorController.resetWheelsMotorPower();

    int angle = 360;
    double speed = 300.0;
    bool isClockwise = true;

    AngleRotation angleRotation(robot, speed, isClockwise, angle);

    double expected = 0.0;  // 回頭しない

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    angleRotation.run();  // 回頭を実行

    // 回頭後に各モータが回転した角度
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    EXPECT_EQ(expected, actual);
  }
}  // namespace etrobocon2025_test
