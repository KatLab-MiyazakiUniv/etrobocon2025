/**
 * @file SpeedCalculatorTest.cpp
 * @brief SpeedCalculatorクラスをテストする
 * @author miyahara046
 */

#include "SpeedCalculator.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {

  // LeftMotorに少しでもPower値が入っているかのテスト
  TEST(SpeedCalculatorTest, CalculateLeftMotorPower)
  {
    Robot robot;
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, 300.0);
    double actualPower = speedCalculator.calculateLeftMotorPower();
    double expected = 0.0;
    EXPECT_LT(expected, actualPower);
  }
  // RightMotorに少しでもPower値が入っているかのテスト
  TEST(SpeedCalculatorTest, CalculateRightMotorPower)
  {
    Robot robot;
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, 300.0);
    double actualPower = speedCalculator.calculateRightMotorPower();
    double expected = 0.0;
    EXPECT_LT(expected, actualPower);
  }

  // LeftMotorに少しでも負のPower値が入っているかのテスト
  TEST(SpeedCalculatorTest, CalculateLeftMotorPoewerFromMinusSpeed)
  {
    Robot robot;
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, -300.0);
    double actualPower = speedCalculator.calculateLeftMotorPower();
    double expected = 0.0;
    EXPECT_GT(expected, actualPower);
  }
  // RightMotorに少しでも負のPower値が入っているかのテスト
  TEST(SpeedCalculatorTest, CalculateRightMotorPoewerFromMinusSpeed)
  {
    Robot robot;
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, -300.0);
    double actualPower = speedCalculator.calculateRightMotorPower();
    double expected = 0.0;
    EXPECT_GT(expected, actualPower);
  }
  // LeftMotorにPower値が入っていないかのテスト
  TEST(SpeedCalculatorTest, CalculateLeftMotorPowerFromZeroSpeed)
  {
    Robot robot;
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, 0.0);
    double actualPower = speedCalculator.calculateLeftMotorPower();
    double expected = 0.0;
    EXPECT_EQ(expected, actualPower);
  }

  // RightMotorにPower値が入っていないかのテスト
  TEST(SpeedCalculatorTest, CalculateRightMotorPowerFromZeroSpeed)
  {
    Robot robot;
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, 0.0);
    double actualPower = speedCalculator.calculateRightMotorPower();
    double expected = 0.0;
    EXPECT_EQ(expected, actualPower);
  }

}  // namespace etrobocon2025_test