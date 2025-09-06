/**
 * @file SpeedCalculatorTest.cpp
 * @brief SpeedCalculatorクラスのテスト
 * @author miyahara046
 */

#include "SpeedCalculator.h"
#include <gtest/gtest.h>
#include "MockSocketClient.h"

namespace etrobocon2025_test {

  // 左右のモーターに少しでもPower値が入っているかのテスト
  TEST(SpeedCalculatorTest, CalculateMotorPower)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, 300.0);
    double actualLeftPower = speedCalculator.calculateLeftMotorPower();
    double actualRightPower = speedCalculator.calculateRightMotorPower();
    double expected = 0.0;
    EXPECT_LT(expected, actualLeftPower);
    EXPECT_LT(expected, actualRightPower);
  }

  // 左右のモーターに少しでも負のPower値が入っているかのテスト
  TEST(SpeedCalculatorTest, CalculateMotorPoewerFromMinusSpeed)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, -300.0);
    double actualLeftPower = speedCalculator.calculateLeftMotorPower();
    double actualRightPower = speedCalculator.calculateRightMotorPower();
    double expected = 0.0;
    EXPECT_GT(expected, actualLeftPower);
    EXPECT_GT(expected, actualRightPower);
  }

  // 左右のモータ―にPower値が入っていないかのテスト
  TEST(SpeedCalculatorTest, CalculateMotorPowerFromZeroSpeed)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    // Powerの初期化
    robot.getMotorControllerInstance().setRightMotorPower(0.0);
    robot.getMotorControllerInstance().setLeftMotorPower(0.0);
    SpeedCalculator speedCalculator(robot, 0.0);
    double actualLeftPower = speedCalculator.calculateLeftMotorPower();
    double actualRightPower = speedCalculator.calculateRightMotorPower();
    double expected = 0.0;
    EXPECT_EQ(expected, actualLeftPower);
    EXPECT_EQ(expected, actualRightPower);
  }
}  // namespace etrobocon2025_test