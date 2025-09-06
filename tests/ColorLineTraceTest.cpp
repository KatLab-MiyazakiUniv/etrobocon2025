/**
 * @file   ColorLineTraceTest.cpp
 * @brief  ColorLineTraceクラスのテスト
 * @author miyahara046
 */

#include "ColorLineTrace.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include "MockSocketClient.h"

using namespace std;

namespace etrobocon2025_test {
  // 最初3回の色取得で連続して指定色を取得するテストケース
  TEST(ColorLineTraceTest, RunToGetFirst)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain);

    double expected = 0.0;
    srand(9037);  // 3回連続して緑を取得する乱数シード
    cl.run();     // 緑までライントレースを実行

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 色を乱数で取得するため、正確な走行距離が予測できず走行距離が進んでいることを確認
    EXPECT_LT(expected, actual);  // 初期値より少しでも進んでいる
  }

  // 目標色が最初から取得され、停止するテストケース
  TEST(ColorLineTraceTest, runImmediatelyStopsIfTargetColorDetected)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 100.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain);

    double expected = 0.0;
    srand(9037);  // 最初に緑を取得できるよう調整
    cl.run();     // ライントレース実行

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 目標色が最初から取得されているため、走行距離は0に近い。継続条件が指定色の取得回数であるためその間走行する。
    EXPECT_NEAR(expected, actual, 30.0);  // 許容誤差を±30.0mmとする
    /**
     * 色所得回数における誤差は以下のとおりである
     * 1回->19.5476mm
     * 2回->21.9911mm
     * 3回->95.2949mm
     */
  }

  // 指定色を取得し、後退するテストケース
  TEST(ColorLineTraceTest, RunBack)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = -500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain);

    double expected = 0.0;
    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 初期値より少しでも後退していることを確認
    EXPECT_LT(actual, expected);
  }

  // targetSpeedが0の時に走行しないテストケース
  TEST(ColorLineTraceTest, RunZeroSpeed)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = 0.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain);

    double expected = 0.0;

    srand(0);
    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 正確に終了している
    EXPECT_EQ(expected, actual);
  }

  // targetColorがNONEの時に走行しないテストケース
  TEST(ColorLineTraceTest, RunNoneColor)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::NONE;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot, targetColor, targetSpeed, targetBrightness, gain);

    double expected = 0.0;

    cl.run();

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 正確に終了している
    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2025_test
