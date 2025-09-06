/**
 * @file   DistanceCameraLineTraceTest.cpp
 * @brief  DistanceCameraLineTraceクラスのテスト
 * @author HaruArima08 miyahara046
 */

#include <gtest/gtest.h>
#include <memory>
#include "DistanceCameraLineTrace.h"
#include "MockSocketClient.h"

#define ERROR 1.01  // 許容誤差の倍率
namespace etrobocon2025_test {
  // 目標距離までカメラライントレースを行うテストケース
  TEST(DistanceCameraLineTraceTest, RunDetectCalled)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain, dummyRequest);

    double expected = targetDistance;

    dcl.run();  // ライントレースを実行

    // カメラライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LE(expected, actual);  // ライントレース後に走行した距離が期待する走行距離以上である
    EXPECT_GT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // targetSpeed値が0の時に終了するテストケース
  TEST(DistanceCameraLineTraceTest, RunZeroSpeed)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    double targetDistance = 1000.0;
    double targetSpeed = 0.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain, dummyRequest);

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
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    double targetDistance = -1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain, dummyRequest);

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
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    double targetDistance = 0.0;
    double targetSpeed = 500.0;
    int targetPoint = 320;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    DistanceCameraLineTrace dcl(robot, targetDistance, targetSpeed, targetPoint, gain, dummyRequest);

    double expected = 0.0;

    dcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }
}  // namespace etrobocon2025_test

