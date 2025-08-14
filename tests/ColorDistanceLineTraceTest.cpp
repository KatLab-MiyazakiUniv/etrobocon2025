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

  // 最初3回の色取得で連続して指定色を取得し、かつ目標距離に到達しない時のテストケース
  TEST(ColorDistanceLineTraceTest, RunToGetFirst)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    COLOR targetColor = COLOR::GREEN;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain);

    double expected = 0.0;  // 走行していない時の走行距離

    // setup hsv
    const spike::HsvResponse HSV_GREEN = { 120, 100, 100 };
    spikeClient.queueHsv({ HSV_GREEN, HSV_GREEN });

    cd.run();  // 緑までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 走行距離が初期値よりも進んでいることを確認
    EXPECT_LT(expected, actual);        // 初期値より少しでも進んでいる
    EXPECT_LT(actual, targetDistance);  // 目標距離までに停止している
  }

  // 少し走ってから指定色を取得し、かつ、目標距離に到達しない時のテストケース
  TEST(ColorDistanceLineTraceTest, ColorRunLeftEdge)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain);

    double expected = 0.0;  // 走行していない時の走行距離

    // setup hsv
    const spike::HsvResponse HSV_BLACK = { 0, 0, 0 };
    const spike::HsvResponse HSV_BLUE = { 240, 100, 100 };
    spikeClient.queueHsv({ HSV_BLACK, HSV_BLACK, HSV_BLACK, HSV_BLUE, HSV_BLUE });

    cd.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 走行距離が初期値よりも進んでいることを確認
    EXPECT_LT(expected, actual);        // 初期値より少しでも進んでいる
    EXPECT_LT(actual, targetDistance);  // 目標距離までに停止している
  }

  // 負のtargetSpeed値で走行し、指定色を取得し、かつ、目標距離に到達しない時のテストケース
  TEST(ColorDistanceLineTraceTest, ColorRunBackLeftEdge)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    COLOR targetColor = COLOR::YELLOW;
    double targetDistance = 1000.0;
    double targetSpeed = -500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain);

    double expected = 0.0;

    srand(0);  // 最初に識別する色が黄ではない乱数シード
    cd.run();  // 黄までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(actual, expected);        // 初期値より後退している
    EXPECT_LT(actual, targetDistance);  // 目標距離までに停止している
  }

  // targetSpeed値が0の時に終了するテストケース
  TEST(ColorDistanceLineTraceTest, RunZeroSpeed)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 0.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain);

    double expected = 0.0;

    srand(0);  // 最初に識別する色が青ではない乱数シード
    cd.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // 目標の色がNONEの時に終了するテストケース
  TEST(ColorDistanceLineTraceTest, RunNoneColorDistance)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    COLOR targetColor = COLOR::NONE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain);

    double expected = 0.0;

    cd.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // 目標距離までライントレースを行い、かつ、指定色を取得できていない時のテストケース
  TEST(ColorDistanceLineTraceTest, DistanceRunLeftEdge)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    COLOR targetColor = COLOR::RED;
    double targetDistance = 100.0;  // カラーをランダムで取得するため短い距離を設定
    double targetSpeed = 500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain);

    double expected = targetDistance;

    srand(1000);  // RED が出にくいシード値にする
    cd.run();     // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // ライントレース後に走行した距離が期待する走行距離である。継続条件の判定の時にsleepを挟むため止まるまでの誤差が生じる
    EXPECT_NEAR(expected, actual, 30.0);  // actualに±30.0mmの誤差を許容
  }

  // targetDistance値が0以下の時に終了するテストケース
  TEST(ColorDistanceLineTraceTest, RunMinusDistance)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    COLOR targetColor = COLOR::RED;
    double targetDistance = -1000.0;
    double targetSpeed = 500.0;
    double targetBrightness = 50.0;
    PidGain gain = { 0.1, 0.05, 0.05 };
    ColorDistanceLineTrace cd(robot, targetColor, targetDistance, targetSpeed, targetBrightness,
                              gain);

    double expected = 0.0;

    cd.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }
}  // namespace etrobocon2025_test