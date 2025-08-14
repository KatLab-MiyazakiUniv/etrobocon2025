/**
 * @file   DistanceLineTraceTest.cpp
 * @brief  DistanceLineTraceクラスのテスト
 * @author nishijima515 miyahara046
 */

#include "DistanceLineTrace.h"
#include "EdgeChange.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

#define ERROR 1.05  // 許容誤差の倍率

namespace etrobocon2025_test {
  // 目標距離までライントレースを行うテストケース（左エッジ走行）
  TEST(DistanceLineTraceTest, RunLeftEdge)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    double targetSpeed = 100.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    // 左エッジに変更
    bool nextEdge = true;
    EdgeChange ec(robot, nextEdge);
    ec.run();

    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain);

    double expected = targetDistance;

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以上である
    EXPECT_GT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // 目標距離までライントレースを行うテストケース（右エッジ走行）
  TEST(DistanceLineTraceTest, RunRightEdge)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    double targetSpeed = 100.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    // エッジを右に変更
    bool nextEdge = false;
    EdgeChange ec(robot, nextEdge);
    ec.run();

    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain);

    double expected = targetDistance;

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以上である
    EXPECT_GT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // 目標距離までライントレースを行うテストケース（バック，左エッジ走行）
  TEST(DistanceLineTraceTest, RunBackLeftEdge)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    double targetSpeed = -100.0;
    double targetDistance = 100.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    // 左エッジに変更
    bool nextEdge = true;
    EdgeChange ec(robot, nextEdge);
    ec.run();

    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain);

    double expected = -targetDistance;

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以下である
    EXPECT_LT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // 目標距離までライントレースを行うテストケース（バック，右エッジ走行）
  TEST(DistanceLineTraceTest, RunBackRightEdge)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    double targetSpeed = -100.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    // 右エッジに変更
    bool nextEdge = false;
    EdgeChange ec(robot, nextEdge);
    ec.run();

    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain);

    double expected = -targetDistance;

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以下である
    EXPECT_LT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // targetSpeed値が0の時に終了するテストケース
  TEST(DistanceLineTraceTest, RunZeroSpeed)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    double targetSpeed = 0.0;
    double targetDistance = 1000.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain);

    double expected = 0.0;
    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // targetDistance値が負の時に終了するテストケース
  TEST(DistanceLineTraceTest, RunMinusDistance)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    double targetSpeed = 100.0;
    double targetDistance = -1000.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain);

    double expected = 0.0;

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // targetDistance値が0のとき終了するテストケース
  TEST(DistanceLineTraceTest, RunZeroDistance)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    double targetSpeed = 500.0;
    double targetDistance = 0.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    DistanceLineTrace dl(robot, targetDistance, targetSpeed, targetBrightness, gain);

    double expected = 0.0;

    dl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }
}  // namespace etrobocon2025_test