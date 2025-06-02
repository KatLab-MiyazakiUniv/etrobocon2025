/**
 * @file   DistanceLineTraceTest.cpp
 * @brief  DistanceLineTraceクラスのテスト
 * @author nishijima515
 */

#include "DistanceLineTrace.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2025_test {
  // 目標距離までライントレースを行うテストケース（左エッジ走行）
  TEST(DistanceLineTraceTest, RunLeftEdge)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetSpeed = 100.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    double basePower = 100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected
    //     = Mileage::calculateMileage(initialRightCount, initialLeftCount) + targetDistance;
    double expected = targetDistance;

    // 一回のsetPower()でダミーのモータカウントに加算される値はpower * 0.05
    double error = Mileage::calculateMileage(basePower * 0.05, basePower * 0.05);  // 許容誤差

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以上である
    EXPECT_GT(expected + error, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // 目標距離までライントレースを行うテストケース（右エッジ走行）
  TEST(DistanceLineTraceTest, RunRightEdge)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetSpeed = 100.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    double basePower = 100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = false;
    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected
    //     = Mileage::calculateMileage(initialRightCount, initialLeftCount) + targetDistance;
    double expected = targetDistance;

    // 一回のsetPower()でダミーのモータカウントに加算される値はpower * 0.05
    double error = Mileage::calculateMileage(basePower * 0.05, basePower * 0.05);  // 許容誤差

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以上である
    EXPECT_GT(expected + error, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // 目標距離までライントレースを行うテストケース（バック，左エッジ走行）
  TEST(DistanceLineTraceTest, RunBackLeftEdge)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetSpeed = -100.0;
    double targetDistance = 100.0;
    double targetBrightness = 45.0;
    double basePower = -100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected
    //     = Mileage::calculateMileage(initialRightCount, initialLeftCount) - targetDistance;
    double expected = -targetDistance;

    // 一回のsetPower()でダミーのモータカウントに加算される値はpower * 0.05
    double error = Mileage::calculateMileage(std::abs(basePower * 0.05),
                                             std::abs(basePower * 0.05));  // 許容誤差

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以下である
    EXPECT_LT(expected - error, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // 目標距離までライントレースを行うテストケース（バック，右エッジ走行）
  TEST(DistanceLineTraceTest, RunBackRightEdge)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetSpeed = -100.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    double basePower = -100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = false;
    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected
    //     = Mileage::calculateMileage(initialRightCount, initialLeftCount) - targetDistance;
    double expected = -targetDistance;

    // 一回のsetPower()でダミーのモータカウントに加算される値はpower * 0.05
    double error = Mileage::calculateMileage(std::abs(basePower * 0.05),
                                             std::abs(basePower * 0.05));  // 許容誤差

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以下である
    EXPECT_LT(expected - error, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // targetSpeed値が0の時にwarning文を出力するテストケース
  TEST(DistanceLineTraceTest, RunZeroSpeed)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetSpeed = 0.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    double basePower = 100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetSpeed value passed to DistanceLineTrace is 0";
    expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

    testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
    dl.run();                                                      // ライントレースを実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
    EXPECT_EQ(expected, actual);              // ライントレース前後で走行距離に変化はない
  }

  // targetDistance値が0以下の時にwarning文を出力するテストケース
  TEST(DistanceLineTraceTest, RunMinusDistance)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetSpeed = 100.0;
    double targetDistance = -1000.0;
    double targetBrightness = 45.0;
    double basePower = 100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetDistance value passed to DistanceLineTrace is -1000.00";
    expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

    testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
    dl.run();                                                      // ライントレースを実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
    EXPECT_EQ(expected, actual);              // ライントレース前後で走行距離に変化はない
  }

  // targetDistance値が0以下かつtargetSpeed値が0のときwarning文を出力するテストケース
  TEST(DistanceLineTraceTest, RunMinusDistanceAndZeroSpeed)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetSpeed = 0.0;
    double targetDistance = -1000.0;
    double targetBrightness = 45.0;
    double basePower = 100.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    // int initialRightCount = measurer.getRightCount();
    // int initialLeftCount = measurer.getLeftCount();
    // double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);
    double expected = 0.0;

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetDistance value passed to DistanceLineTrace is "
                      "-1000.00, and the targetSpeed "
                      "value passed "
                      "to ColorLineTrace is 0";
    expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

    testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
    dl.run();                                                      // ライントレースを実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
    EXPECT_EQ(expected, actual);              // ライントレース前後で走行距離に変化はない
  }
}  // namespace etrobocon2025_test