/**
 *  @file   DistanceStraightTest.cpp
 *  @brief  DistanceStraightクラスのテスト
 *  @author HaruArima08
 */

#include "DistanceStraight.h"
#include <gtest/gtest.h>
#include <iostream>

#define MILEAGE_ACCEPTABLE_ERROR 10.0

using namespace std;

namespace etrobocon2025_test {

  // 通常速度での直進テスト
  TEST(DistanceStraightTest, RunNomalSpeed)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = 300.0;
    double basePower = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = targetDistance;

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = rightActual - rightInitial;
    double leftDifference = leftActual - leftInitial;
    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_GE(actualDistance, expectedDistance);  // 目標距離以上進んでいるか
    EXPECT_LE(actualDistance,
              expectedDistance + MILEAGE_ACCEPTABLE_ERROR);  // 許容誤差内に収まっているか
    // 左右タイヤの走行距離差が0に近い（＝直進できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, targetDistance / 10.0);
  }

  // 十分大きい速度での直進テスト
  TEST(DistanceStraightTest, RunFullPower)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = 600.0;
    double basePower = 100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = targetDistance;

    ds.run();  // 直進を実行

    // 直進後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 直進後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = rightActual - rightInitial;
    double leftDifference = leftActual - leftInitial;
    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_GE(actualDistance, expectedDistance);  // 目標距離以上進んでいるか
    EXPECT_LE(actualDistance,
              expectedDistance + MILEAGE_ACCEPTABLE_ERROR);  // 許容誤差内に収まっているか
    // 左右タイヤの走行距離差が0に近い（＝直進できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, targetDistance / 10.0);
  }

  // 後退テスト
  TEST(DistanceStraightTest, RunBack)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = -300.0;
    double basePower = -100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = -targetDistance;

    ds.run();  // 後退を実行

    // 後退後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 後退後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = rightActual - rightInitial;
    double leftDifference = leftActual - leftInitial;
    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_LE(actualDistance, expectedDistance);  // 目標距離以上進んでいるか
    EXPECT_GE(actualDistance,
              expectedDistance - MILEAGE_ACCEPTABLE_ERROR);  // 許容誤差内に収まっているか
    // 左右タイヤの走行距離差が0に近い（＝直進できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, targetDistance / 10.0);
  }

  // 十分大きい速度での後退テスト
  TEST(DistanceStraightTest, RunBackFullPower)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    // Power値の初期化
    robot.getMotorControllerInstance().resetWheelsMotorPower();
    double targetDistance = 1000.0;
    double targetSpeed = -600.0;
    double basePower = -100.0;
    DistanceStraight ds(robot, targetDistance, targetSpeed);

    // 初期値 （タイヤの累計走行距離）
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);

    // 期待する走行距離
    double expectedDistance = -targetDistance;

    ds.run();  // 後退を実行

    // 後退後の走行距離
    rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // 後退後のタイヤの累計走行距離
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    // タイヤごとの走行距離
    double rightDifference = rightActual - rightInitial;
    double leftDifference = leftActual - leftInitial;
    // タイヤの走行距離の誤差
    double actualError = std::abs(rightDifference - leftDifference);

    // 走行距離のテスト
    EXPECT_LE(actualDistance, expectedDistance);  // 目標距離以上進んでいるか
    EXPECT_GE(actualDistance,
              expectedDistance - MILEAGE_ACCEPTABLE_ERROR);  // 許容誤差内に収まっているか
    // 左右タイヤの走行距離差が0に近い（＝後退できているか）のテスト
    EXPECT_NEAR(actualError, 0.0, targetDistance / 10.0);
  }

  // 目標距離が負のとき停止するかのテスト
  TEST(DistanceStraightTest, RunMinusDistance)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
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
    SpikeClient spikeClient;
    Robot robot(spikeClient);
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
    SpikeClient spikeClient;
    Robot robot(spikeClient);
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
    SpikeClient spikeClient;
    Robot robot(spikeClient);
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