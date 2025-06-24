/**
 * @file   DistanceCameraLineTraceTest.cpp
 * @brief  DistanceCameraLineTraceクラスのテスト
 * @author HaruArima08 miyahara046
 */

#include <gtest/gtest.h>
#include <memory>
#include "DistanceCameraLineTrace.h"
#include "DummyBoundingBoxDetector.h"
#include "DummyCameraCapture.h"

#define ERROR 1.01  // 許容誤差の倍率

namespace etrobocon2025_test {
  // 目標距離までカメラライントレースを行うテストケース
  TEST(DistanceCameraLineTraceTest, RunDetectCalled)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = std::make_unique<DummyBoundingBoxDetector>();

    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain,
                                std::move(detector));

    double expected = targetDistance;

    dcl.run();  // ライントレースを実行

    // カメラライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以上である
    EXPECT_GT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // フレーム取得に指定回数以上失敗した場合に終了するテストケース
  TEST(DistanceCameraLineTraceTest, RunStopFrameFailures)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = std::make_unique<DummyBoundingBoxDetector>();

    // DummyCameraCaptureで3回連続失敗を設定
    cameraCapture.setFrameResults({ true, true, false, false, false });

    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain,
                                std::move(detector));

    dcl.run();  // ライントレースを実行

    // 実際の走行距離を取得
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // 走行距離が目標距離未満であることを確認
    EXPECT_LT(actual, targetDistance);
  }

  // targetSpeed値が0の時に終了するテストケース
  TEST(DistanceCameraLineTraceTest, RunZeroSpeed)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetDistance = 1000.0;
    double targetSpeed = 0.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = std::make_unique<DummyBoundingBoxDetector>();

    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain,
                                std::move(detector));

    double expected = 0.0;
    dcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // targetDistance値が負の時に終了するテストケース
  TEST(DistanceCameraLineTraceTest, RunMinusDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetDistance = -1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = std::make_unique<DummyBoundingBoxDetector>();

    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain,
                                std::move(detector));

    double expected = 0.0;

    dcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  // targetDistance値が0のとき終了するテストケース
  TEST(DistanceCameraLineTraceTest, RunZeroDistance)
  {
    DummyCameraCapture cameraCapture;
    Robot robot(cameraCapture);
    double targetDistance = 0.0;
    double targetSpeed = 500.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    auto detector = std::make_unique<DummyBoundingBoxDetector>();

    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain,
                                std::move(detector));

    double expected = 0.0;

    dcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }
}  // namespace etrobocon2025_test
