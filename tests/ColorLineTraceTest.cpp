/**
 * @file   ColorLineTraceTest.cpp
 * @brief  ColorLineTraceクラスのテスト
 * @author miyahara046
 */

#include "ColorLineTrace.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2025_test {
  // 最初3回の色取得で連続して指定色を取得するテストケース
  TEST(ColorLineTraceTest, runToGetFirst)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 50000.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;
    srand(9037);  // 3回連続して緑を取得する乱数シード
    cl.run();     // 緑までライントレースを実行

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);  // 初期値より少しでも進んでいる
  }

  // 目標色が最初から取得され、停止するテストケース
  TEST(ColorLineTraceTest, runImmediatelyStopsIfTargetColorDetected)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;
    srand(9037);  // 最初に緑を取得できるよう調整
    cl.run();     // ライントレース実行

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 目標色が最初から取得されているため、走行距離は0に近い。走行開始してから判定するため走行距離が0ではないことに注意。
    EXPECT_NEAR(expected, actual, 60.0);  // 許容誤差を60mmとする
  }

  // 以下のテストケースはそのまま再利用（書式を既に整えているため）
  TEST(ColorLineTraceTest, runLeftEdge)
  {
    Robot robot;
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;
    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);
  }

  TEST(ColorLineTraceTest, runRightEdge)
  {
    Robot robot;
    COLOR targetColor = COLOR::RED;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = false;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;
    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);
  }

  TEST(ColorLineTraceTest, runBackLeftEdge)
  {
    Robot robot;
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = -100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;
    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected, actual);
  }

  TEST(ColorLineTraceTest, runBackLeftEdgeWithZeroSpeed)
  {
    Robot robot;
    COLOR targetColor = COLOR::YELLOW;
    double targetSpeed = -100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;
    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected, actual);
  }

  TEST(ColorLineTraceTest, runBackRightEdge)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = -100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = false;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;
    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected, actual);
  }

  TEST(ColorLineTraceTest, runZeroSpeed)
  {
    Robot robot;
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = 0.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;

    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);
  }

  TEST(ColorLineTraceTest, runNoneColor)
  {
    Robot robot;
    COLOR targetColor = COLOR::NONE;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;

    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);
  }

  TEST(ColorLineTraceTest, runNoneColorAndZeroSpeed)
  {
    Robot robot;
    COLOR targetColor = COLOR::NONE;
    double targetSpeed = 0.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    double expected = 0.0;

    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);
  }
}  // namespace etrobocon2025_test
