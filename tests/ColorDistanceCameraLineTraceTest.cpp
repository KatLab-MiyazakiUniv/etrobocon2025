/**
 * @file   ColorDistanceCameraLineTraceTest.cpp
 * @brief  ColorDistanceCameraLineTraceクラスのテスト
 * @author HaruArima08
 */

#include "ColorDistanceCameraLineTrace.h"
#include "DummyBoundingBoxDetector.h"
#include "DummyCameraCapture.h"
#include <gtest/gtest.h>

#define ERROR 1.01  // 許容誤差の倍率

using namespace std;

namespace etrobocon2025_test {

  // 走行直後に3回の色取得で連続して指定色を取得し、かつ目標距離に到達しない時のテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunToGetColorImmediate)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, move(detector));

    double expected = 0.0;  // 走行していない時の走行距離

    srand(9037);  // 3回連続して青を取得する乱数シード

    cdcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);        // 走行距離が初期値より少し進んでいる
    EXPECT_LT(actual, targetDistance);  // 目標距離未満で停止している
  }

  // 少し走行後、指定色を取得し、かつ目標距離に到達しない時のテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunToGetColorDelayed)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, move(detector));

    double expected = 0.0;  // 走行していない時の走行距離

    srand(0);  // 最初に識別する色が青ではない乱数シード

    cdcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);        // 走行距離が初期値より少し進んでいる
    EXPECT_LT(actual, targetDistance);  // 目標距離までに停止している
  }

  // 負のtargetSpeed値で走行しつつ指定色を取得し、かつ目標距離に到達しない時のテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunBackToGetColor)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = -500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, move(detector));

    double expected = 0.0;  // 走行していない時の走行距離

    srand(0);  // 最初に識別する色が青ではない乱数シード

    cdcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(actual, expected);        // 初期値より後退している
    EXPECT_LT(actual, targetDistance);  // 目標距離までに停止している
  }

  // 目標距離までライントレースを行い、かつ指定色を取得できていない時のテストケース
  TEST(ColorDistanceCameraLineTraceTest, DistanceRunNoGetColor)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 100.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, move(detector));

    double expected = targetDistance;

    srand(1500);  // BLUE が出にくいシード値

    cdcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // targetSpeed値が0の時に終了するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunZeroSpeed)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 0.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, move(detector));
    double expected = 0.0;

    srand(0);  // 最初に識別する色が青ではない乱数シード

    cdcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // 目標の色がNONEの時に終了するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunNoneColor)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    COLOR targetColor = COLOR::NONE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, move(detector));

    double expected = 0.0;

    cdcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // targetDistance値が0以下の時に終了するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunMinusDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 0.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, move(detector));
    double expected = 0.0;

    srand(0);  // 最初に識別する色が青ではない乱数シード

    cdcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }
}  // namespace etrobocon2025_test