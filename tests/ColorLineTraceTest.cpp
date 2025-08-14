/**
 * @file   ColorLineTraceTest.cpp
 * @brief  ColorLineTraceクラスのテスト
 * @author miyahara046
 */

#include "ColorLineTrace.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

namespace etrobocon2025_test {
  // 色のHSV値を定義
  const spike::HsvResponse HSV_BLACK = { 0, 0, 0 };
  const spike::HsvResponse HSV_GREEN = { 120, 100, 100 };
  const spike::HsvResponse HSV_BLUE = { 240, 100, 100 };

  // ColorLineTraceのテストクラス
  class ColorLineTraceTest : public ::testing::Test {
   protected:
    SpikeClient spikeClient_;
    Robot robot_{ spikeClient_ };
  };

  // 継続条件が2回なので、3回目で停止することを期待するテスト
  TEST_F(ColorLineTraceTest, RunAndStopAfterFindingColor)
  {
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 600.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot_, targetColor, targetSpeed, targetBrightness, gain);

    // センサー値のシーケンスを設定
    spikeClient_.queueHsv({ HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_GREEN, HSV_GREEN });

    cl.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 走行距離が0より大きいことを確認
    EXPECT_LT(0.0, actual);
  }

  // 即座に停止するテスト
  TEST_F(ColorLineTraceTest, runImmediatelyStopsIfTargetColorDetected)
  {
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 600.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot_, targetColor, targetSpeed, targetBrightness, gain);

    // 最初に2回連続で緑を読み、すぐに停止するシナリオ
    spikeClient_.queueHsv({ HSV_GREEN, HSV_GREEN });

    cl.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 走行距離はほぼ0のはず
    EXPECT_NEAR(0.0, actual, 30.0);  // 許容誤差を±30.0mmとする
  }

  // 後退するテスト
  TEST_F(ColorLineTraceTest, RunBack)
  {
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = -600.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot_, targetColor, targetSpeed, targetBrightness, gain);

    // センサー値のシーケンスを設定
    spikeClient_.queueHsv({ HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLUE, HSV_BLUE });

    cl.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 走行距離が負（後退）であることを確認
    EXPECT_GT(0.0, actual);
  }

  // targetSpeedが0の時に走行しないテスト
  TEST_F(ColorLineTraceTest, RunZeroSpeed)
  {
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = 0.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot_, targetColor, targetSpeed, targetBrightness, gain);

    cl.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(0.0, actual);
  }

  // targetColorがNONEの時に走行しないテスト
  TEST_F(ColorLineTraceTest, RunNoneColor)
  {
    COLOR targetColor = COLOR::NONE;
    double targetSpeed = 600.0;
    double targetBrightness = 45.0;
    PidGain gain = { 0.1, 0.05, 0.05 };

    ColorLineTrace cl(robot_, targetColor, targetSpeed, targetBrightness, gain);

    cl.run();

    int rightCount = robot_.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot_.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(0.0, actual);
  }

}  // namespace etrobocon2025_test