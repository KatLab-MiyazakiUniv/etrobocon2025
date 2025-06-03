/**
 * @file   ColorDistanceLineTraceTest.cpp
 * @brief  ColorDistanceLineTraceクラスのテスト
 * @author miyahara046
 */

#include "ColorDistanceLineTrace.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2025_test {
  // 最初3回の色取得で連続して指定色を取得するテストケース
  TEST(ColorDistanceLineTraceTest, runToGetFirst)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetDistance = 10000;
    double targetSpeed = 50000.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;
    srand(9037);  // 3回連続して緑を取得する乱数シード
    cd.run();     // 緑までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);  // 初期値より少しでも進んでいる
    EXPECT_LT(actual, targetDistance);
  }

  // 少し走ってから指定色を取得するテストケース
  TEST(ColorDistanceLineTraceTest, colorRunLeftEdge)
  {
    Robot robot;
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 10000;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    srand(0);  // 最初に識別する色が青ではない乱数シード
    cd.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);        // 実行後に少しでも進んでいる
    EXPECT_LT(actual, targetDistance);  // 目標距離よりも進んでいない
  }

  // 後退するテストケース
  TEST(ColorDistanceLineTraceTest, colorRunBackLeftEdge)
  {
    Robot robot;
    COLOR targetColor = COLOR::YELLOW;
    double targetDistance = 1000;
    double targetSpeed = -100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    srand(0);  // 最初に識別する色が黄ではない乱数シード
    cd.run();  // 黄までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(actual, expected);              // 実行後に少しでも進んでいる
    EXPECT_LT(fabs(actual), targetDistance);  // 目標距離よりも進んでいない
  }

  // targetSpeed値が0の時に終了するテストケース
  TEST(ColorDistanceLineTraceTest, runZeroSpeed)
  {
    Robot robot;
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 10000;
    double targetSpeed = 0.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    srand(0);  // 最初に識別する色が青ではない乱数シード
    cd.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // 目標の色がNONEの時に終了するテストケース
  TEST(ColorDistanceLineTraceTest, runNoneColorDistance)
  {
    Robot robot;
    COLOR targetColor = COLOR::NONE;
    double targetDistance = 10000;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = robot.getMotorControllerInstance().getRightMotorCount();
    // int initialLeftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    cd.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // 目標の色がNoneかつtargetSpeed値が0のとき終了するテストケース
  TEST(ColorDistanceLineTraceTest, runNoneColorDistanceAndZeroSpeed)
  {
    Robot robot;
    COLOR targetColor = COLOR::NONE;
    double targetDistance = 0.0;
    double targetSpeed = 0.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = robot.getMotorControllerInstance().getRightMotorCount();
    // int initialLeftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    cd.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // 目標距離までライントレースを行うテストケース
  TEST(ColorDistanceLineTraceTest, DistanceRunLeftEdge)
  {
    Robot robot;
    COLOR targetColor = COLOR::RED;
    double targetSpeed = 100.0;
    double targetDistance = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = robot.getMotorControllerInstance().getRightCount();
    // int initialLeftCount = robot.getMotorControllerInstance().getLeftCount();
    // double expected
    //     = Mileage::calculateMileage(initialRightCount, initialLeftCount) + targetDistance;
    double expected = targetDistance;

    srand(1234);  // RED が出にくいシード値にする

    cd.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // ライントレース後に走行した距離が期待する走行距離である。ライントレース後であるため止まるまでの誤差が生じる
    EXPECT_NEAR(expected, actual, 1.0);
  }

  // targetDistance値が0以下の時に終了するテストケース
  TEST(ColorDistanceLineTraceTest, runMinusDistance)
  {
    Robot robot;
    COLOR targetColor = COLOR::RED;
    double targetSpeed = 100.0;
    double targetDistance = -1000.0;
    double targetBrightness = 45.0;
    double basePwm = 100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = robot.getMotorControllerInstance().getRightCount();
    // int initialLeftCount = robot.getMotorControllerInstance().getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    cd.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // targetDistance値が0以下かつtargetSpeed値が0のとき終了するテストケース
  TEST(ColorDistanceLineTraceTest, runMinusDistanceAndZeroSpeed)
  {
    Robot robot;
    COLOR targetColor = COLOR::RED;
    double targetSpeed = 0.0;
    double targetDistance = -50.0;
    double targetBrightness = 45.0;
    double basePwm = 100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = robot.getMotorControllerInstance().getRightCount();
    // int initialLeftCount = robot.getMotorControllerInstance().getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    cd.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }
}  // namespace etrobocon2025_test