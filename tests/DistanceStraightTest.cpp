/**
 *  @file   DistanceStraightTest.cpp
 *  @brief  DistanceStraightクラスのテスト
 *  @author HaruArima08
 */

#include "DistanceStraight.h"
#include <gtest/gtest.h>

using namespace std;

namespace etrobocon2025_test {

  // 通常速度での直進テスト
  TEST(DistanceStraightTest, Run)
  {
    Robot robot;

    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = 50.0;
    double dummyPower = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getRightMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = targetDistance;

    // 一回のsetPowewr()でダミーのモータカウントに加算される値はpower * 0.05
    double distanceError
        = Mileage::calculateMileage(dummyPower * 0.05, dummyPower * 0.05);  // 許容誤差

    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);

    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - leftInitial);
    double leftDifference = std::abs(leftActual - rightInitial);

    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_NEAR(expectedDistance, actualDistance, distanceError);
    // 直進できているかのテスト
    EXPECT_LE(actualError, expectedError);
  }

  // 最大速度での直進テスト
  TEST(DistanceStraightTest, RunFullPower)
  {
    Robot robot;

    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = 100.0;
    double dummyPower = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getRightMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = targetDistance;

    // 一回のsetPowewr()でダミーのモータカウントに加算される値はpower * 0.05
    double distanceError
        = Mileage::calculateMileage(dummyPower * 0.05, dummyPower * 0.05);  // 許容誤差

    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);

    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - leftInitial);
    double leftDifference = std::abs(leftActual - rightInitial);

    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_NEAR(expectedDistance, actualDistance, distanceError);
    // 直進できているかのテスト
    EXPECT_LE(actualError, expectedError);
  }

  // 後退テスト
  TEST(DistanceStraightTest, RunBack)
  {
    Robot robot;

    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = -50.0;
    double dummyPower = -100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getRightMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) - targetDistance;

    // 一回のsetPowewr()でダミーのモータカウントに加算される値はpower * 0.05
    double distanceError
        = std::abs(Mileage::calculateMileage(dummyPower * 0.05, dummyPower * 0.05));  // 許容誤差
    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);

    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - leftInitial);
    double leftDifference = std::abs(leftActual - rightInitial);

    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_NEAR(expectedDistance, actualDistance, distanceError);
    // 直進できているかのテスト
    EXPECT_LE(actualError, expectedError);
  }

  // 最大速度での後退テスト
  TEST(DistanceStraightTest, RunBackFullPower)
  {
    Robot robot;

    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = -100.0;
    double dummyPower = -100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getRightMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) - targetDistance;

    // 一回のsetPowewr()でダミーのモータカウントに加算される値はpower * 0.05
    double distanceError
        = std::abs(Mileage::calculateMileage(dummyPower * 0.05, dummyPower * 0.05));  // 許容誤差
    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);

    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - leftInitial);
    double leftDifference = std::abs(leftActual - rightInitial);

    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_NEAR(expectedDistance, actualDistance, distanceError);
    // 直進できているかのテスト
    EXPECT_LE(actualError, expectedError);
  }

  // 目標距離が負のとき警告＋停止するかのテスト
  TEST(DistanceStraightTest, RunMinusDistance)
  {
    Robot robot;

    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = -1000.0;
    double targetSpeed = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getRightMotorCount();

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    // Warning文
    string expectedOutput = "[Warning] targetDistance is less than or equal to 0.0\n";

    testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
    ds.run();                                                      // 直進を実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 直進実行後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);      // 標準出力でWarningを出している
    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

  // 目標速度が0で警告＋停止するかのテスト
  TEST(DistanceStraightTest, RunZeroPower)
  {
    Robot robot;

    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = 0.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getRightMotorCount();

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    // Warning文
    string expectedOutput = "[Warning] targetSpeed is 0.0\n";

    testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
    ds.run();                                                      // 直進を実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 直進実行後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);      // 標準出力でWarningを出している
    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

  // 距離・速度共に異常時の動作確認テスト
  TEST(DistanceStraightTest, RunMinusDistanceZeroPower)
  {
    Robot robot;

    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = -1000.0;
    double targetSpeed = 0.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getRightMotorCount();

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    // Warning文
    string expectedOutput = "[Warning] targetSpeed is 0.0\n";

    testing::internal::CaptureStdout();                            // 標準出力キャプチャ開始
    ds.run();                                                      // 直進を実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 直進実行後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);      // 標準出力でWarningを出している
    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

}  // namespace etrobocon2025_test