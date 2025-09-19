/**
 * @file   UltrasonicDistanceCameraLineTraceTest.cpp
 * @brief  UltrasonicDistanceCameraLineTraceクラスのテスト
 * @author nishijima515
 */

#include "UltrasonicDistanceCameraLineTrace.h"
#include "DummyBoundingBoxDetector.h"
#include "DummyCameraCapture.h"
#include <gtest/gtest.h>

#define ERROR 1.01  // 許容誤差の倍率

using namespace std;

namespace etrobocon2025_test {

  // 走行後に超音波指定距離に到達し、かつ目標距離に到達しない場合、走行距離が初期値より進み、目標距離未満で停止することを確認するテストケース
  TEST(UltrasonicDistanceCameraLineTraceTest, RunToGetUltrasonicDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetUltrasonicDistance = 60.0;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    UltrasonicDistanceCameraLineTrace udcl(robot, targetUltrasonicDistance, targetDistance,
                                           targetSpeed, targetPoint, gain, move(detector));

    double expected = 0.0;  // 走行していない時の走行距離
    srand(0);               // 最初に70mmが出る乱数シード
    udcl.run();             // 超音波指定距離までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);        // 走行距離が初期値より少し進んでいる
    EXPECT_LT(actual, targetDistance);  // 目標距離までに停止している
  }

  // 負のtargetSpeed値で走行しつつ超音波指定距離を取得し、かつ目標距離に到達しない場合、初期値より後退し、目標距離未満で停止することを確認するテストケース
  TEST(UltrasonicDistanceCameraLineTraceTest, RunBackToGetUltrasonicDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetUltrasonicDistance = 30.0;
    double targetDistance = 1000.0;
    double targetSpeed = -500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    UltrasonicDistanceCameraLineTrace udcl(robot, targetUltrasonicDistance, targetDistance,
                                           targetSpeed, targetPoint, gain, move(detector));

    double expected = 0.0;  // 走行していない時の走行距離

    srand(7);  // 最初に10mmが出る乱数シード

    udcl.run();  // 超音波指定距離までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(actual, expected);        // 初期値より後退している
    EXPECT_LT(actual, targetDistance);  // 目標距離までに停止している
  }

  // 超音波指定距離を取得できないまま目標距離に到達した場合、実際の走行距離が目標距離の許容誤差以内であることを確認するテストケース
  TEST(UltrasonicDistanceCameraLineTraceTest, DistanceRunNoGetUltrasonicDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetUltrasonicDistance = 5.0;
    double targetDistance = 100.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    UltrasonicDistanceCameraLineTrace udcl(robot, targetUltrasonicDistance, targetDistance,
                                           targetSpeed, targetPoint, gain, move(detector));

    double expected = targetDistance;

    udcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // targetSpeed値が0の場合、停止することを確認するテストケース
  TEST(UltrasonicDistanceCameraLineTraceTest, RunZeroSpeed)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);

    double targetUltrasonicDistance = 5.0;
    double targetDistance = 1000.0;
    double targetSpeed = 0.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    UltrasonicDistanceCameraLineTrace udcl(robot, targetUltrasonicDistance, targetDistance,
                                           targetSpeed, targetPoint, gain, move(detector));
    double expected = 0.0;

    udcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // 超音波指定距離が0以下の場合、停止することを確認するテストケース
  TEST(UltrasonicDistanceCameraLineTraceTest, RunZeroDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetUltrasonicDistance = 0.0;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    UltrasonicDistanceCameraLineTrace udcl(robot, targetUltrasonicDistance, targetDistance,
                                           targetSpeed, targetPoint, gain, move(detector));

    double expected = 0.0;

    udcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // targetDistance値が0以下の場合、停止することを確認するテストケース
  TEST(UltrasonicDistanceCameraLineTraceTest, RunMinusDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetUltrasonicDistance = 5.0;
    double targetDistance = 0.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = make_unique<DummyBoundingBoxDetector>();

    UltrasonicDistanceCameraLineTrace udcl(robot, targetUltrasonicDistance, targetDistance,
                                           targetSpeed, targetPoint, gain, move(detector));
    double expected = 0.0;

    udcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }
}  // namespace etrobocon2025_test