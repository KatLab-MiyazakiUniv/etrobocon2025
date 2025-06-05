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
  TEST(DistanceStraightTest, RunNomalSpeed)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = 50.0;
    double basePower = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = targetDistance;

    // モーターに1回setMotorPower()を呼ぶと、モータカウントが power × 0.05分進む
    // 1ステップ分の走行距離を許容誤差とする
    double distanceError
        = Mileage::calculateMileage(basePower * 0.05, basePower * 0.05);  // 許容誤差

    double expectedError = 1.0;  // タイヤの走行距離の許容誤差

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - rightInitial);
    double leftDifference = std::abs(leftActual - leftInitial);
    // タイヤの走行距離の誤差
    double actualError = (rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_LE(actualDistance, expectedDistance + distanceError);
    // 左右タイヤの走行距離差が0に近い（＝直進できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, expectedError);
  }

  // 十分大きい速度での直進テスト
  TEST(DistanceStraightTest, RunFullPower)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = 1000.0;
    double basePower = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = targetDistance;

    // モーターに1回setMotorPower()を呼ぶと、モータカウントが power × 0.05分進む
    // 1ステップ分の走行距離を許容誤差とする
    double distanceError
        = Mileage::calculateMileage(basePower * 0.05, basePower * 0.05);  // 許容誤差

    double expectedError = 1.0;  // タイヤの走行距離の許容誤差

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - rightInitial);
    double leftDifference = std::abs(leftActual - leftInitial);
    // タイヤの走行距離の誤差
    double actualError = (rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_LE(actualDistance, expectedDistance + distanceError);
    // 左右タイヤの走行距離差が0に近い（＝直進できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, expectedError);
  }

  // 後退テスト
  TEST(DistanceStraightTest, RunBack)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = -50.0;
    double basePower = -100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) - targetDistance;

    // モーターに1回setMotorPower()を呼ぶと、モータカウントが power × 0.05分進む
    // 1ステップ分の走行距離を許容誤差とする
    double distanceError
        = std::abs(Mileage::calculateMileage(basePower * 0.05, basePower * 0.05));  // 許容誤差

    double expectedError = 1.0;  // タイヤの走行距離の許容誤差

    ds.run();  // 後退を実行

    // 後退後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 後退後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - rightInitial);
    double leftDifference = std::abs(leftActual - leftInitial);
    // タイヤの走行距離の誤差
    double actualError = (rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_LE(actualDistance, expectedDistance + distanceError);
    // 左右タイヤの走行距離差が0に近い（＝後退できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, expectedError);
  }

  // 十分大きい速度での後退テスト
  TEST(DistanceStraightTest, RunBackFullPower)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = -1000.0;
    double basePower = -100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) - targetDistance;

    // モーターに1回setMotorPower()を呼ぶと、モータカウントが power × 0.05分進む
    // 1ステップ分の走行距離を許容誤差とする
    double distanceError
        = std::abs(Mileage::calculateMileage(basePower * 0.05, basePower * 0.05));  // 許容誤差

    double expectedError = 1.0;  // タイヤの走行距離の許容誤差

    ds.run();  // 後退を実行

    // 後退後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 後退後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = std::abs(rightActual - rightInitial);
    double leftDifference = std::abs(leftActual - leftInitial);
    // タイヤの走行距離の誤差
    double actualError = (rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_LE(actualDistance, expectedDistance + distanceError);
    // 左右タイヤの走行距離差が0に近い（＝後退できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, expectedError);
  }

  // 目標距離が負のとき停止するかのテスト
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
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    ds.run();  // 直進を実行

    // 直進実行後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

  // 目標距離が0のとき停止するかのテスト
  TEST(DistanceStraightTest, RunZeroDistance)
  {
    Robot robot;
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 0.0;
    double targetSpeed = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    ds.run();  // 直進を実行

    // 直進実行後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

  // 目標速度が0のとき停止するかのテスト
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
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    ds.run();  // 直進を実行

    // 直進実行後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

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
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();

    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    ds.run();  // 直進を実行

    // 直進実行後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

}  // namespace etrobocon2025_test