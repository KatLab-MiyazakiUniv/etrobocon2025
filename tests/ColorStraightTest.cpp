/**
 *  @file   ColorStraightTest.cpp
 *  @brief  ColorStraightクラスのテスト
 *  @author HaruArima08
 */

#include "ColorStraight.h"
#include "Mileage.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {

  // 色のHSV値を定義
  const spike::HsvResponse HSV_BLACK = { 0, 0, 0 };
  const spike::HsvResponse HSV_GREEN = { 120, 100, 100 };
  const spike::HsvResponse HSV_YELLOW = { 60, 100, 100 };

  class ColorStraightTest : public ::testing::Test {
   protected:
    SpikeClient spikeClient_;
    Robot robot_{ spikeClient_ };
  };

  // 少し直進して指定色を検出し停止するテスト
  TEST_F(ColorStraightTest, RunAndStop)
  {
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed
        = 600.0;  // スピードカルクのpidが小さすぎて、カウントが動く前に色を検出してしまうため、speedを極端に大きくする必要がある。
    ColorStraight cs(robot_, targetColor, targetSpeed);

    // 一時前進し、その後3回緑を読んで停止するシナリオ
    spikeClient_.queueHsv(
        { HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_GREEN, HSV_GREEN, HSV_GREEN });

    cs.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    double expected = 0.0;
    EXPECT_LT(expected, actual);  // 初期値より少しは進んでいる
  }

  // 指定色を即検出し、すぐに停止するテスト
  TEST_F(ColorStraightTest, StopImmediately)
  {
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 300.0;
    ColorStraight cs(robot_, targetColor, targetSpeed);

    // 最初に3回連続で緑を読み、すぐに停止するシナリオ
    spikeClient_.queueHsv({ HSV_GREEN, HSV_GREEN, HSV_GREEN });

    cs.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    double expected = 0.0;
    // 走行距離はほぼ0のはず
    EXPECT_NEAR(expected, actual, 30.0);
  }

  // 少し後退して指定色を取得するテスト
  TEST_F(ColorStraightTest, RunBackAndStop)
  {
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = -600.0;
    ColorStraight cs(robot_, targetColor, targetSpeed);

    // 一時後進し、その後3回緑を読んで停止するシナリオ
    spikeClient_.queueHsv(
        { HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK,
          HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_GREEN, HSV_GREEN, HSV_GREEN });

    cs.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    double expected = 0.0;
    // 走行距離が負（後退）であることを確認
    EXPECT_GT(expected, actual);
  }

  // 目標速度が0で走行しないことを確認するテスト
  TEST_F(ColorStraightTest, RunZeroSpeed)
  {
    COLOR targetColor = COLOR::YELLOW;
    double targetSpeed = 0.0;
    ColorStraight cs(robot_, targetColor, targetSpeed);

    cs.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    double expected = 0.0;
    EXPECT_EQ(expected, actual);
  }

  // 目標色がNONEで走行しないことを確認するテスト
  TEST_F(ColorStraightTest, RunNoneColor)
  {
    COLOR targetColor = COLOR::NONE;
    double targetSpeed = 100.0;
    ColorStraight cs(robot_, targetColor, targetSpeed);

    cs.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    double expected = 0.0;
    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2025_test
