/**
 * @file   ColorDistanceCameraLineTraceTest.cpp
 * @brief  ColorDistanceCameraLineTraceクラスのテスト
 * @author HaruArima08 nishijima515
 */

#include "ColorDistanceCameraLineTrace.h"
#include "MockSocketClient.h"
#include <gtest/gtest.h>

#define ERROR 1.01  // 許容誤差の倍率

using namespace std;

namespace etrobocon2025_test {

  // 走行直後に指定色を3回連続取得し、かつ目標距離に到達しない場合、走行距離が初期値より少し進み、目標距離未満で停止することを確認するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunToGetColorImmediate)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    successResponse.result.x = 320;
    successResponse.result.y = 240;
    successResponse.result.width = 100;
    successResponse.result.height = 100;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, dummyRequest);

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

  // 少し走行後に指定色を取得し、かつ目標距離に到達しない場合、走行距離が初期値より進み、目標距離未満で停止することを確認するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunToGetColorDelayed)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    successResponse.result.x = 320;
    successResponse.result.y = 240;
    successResponse.result.width = 100;
    successResponse.result.height = 100;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, dummyRequest);

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

  // 負のtargetSpeed値で走行しつつ指定色を取得し、かつ目標距離に到達しない場合、初期値より後退し、目標距離未満で停止することを確認するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunBackToGetColor)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = -500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    successResponse.result.x = 320;
    successResponse.result.y = 240;
    successResponse.result.width = 100;
    successResponse.result.height = 100;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, dummyRequest);

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

  // 指定色を取得できないまま目標距離に到達した場合、実際の走行距離が目標距離の許容誤差以内であることを確認するテストケース
  TEST(ColorDistanceCameraLineTraceTest, DistanceRunNoGetColor)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 100.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    successResponse.result.x = 320;
    successResponse.result.y = 240;
    successResponse.result.width = 100;
    successResponse.result.height = 100;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, dummyRequest);

    double expected = targetDistance;

    srand(1500);  // BLUE が出にくいシード値

    cdcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected * ERROR, actual);  // ライントレース後に走行した距離が許容誤差未満である
  }

  // targetSpeed値が0の場合、停止することを確認するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunZeroSpeed)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 1000.0;
    double targetSpeed = 0.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    successResponse.result.x = 320;
    successResponse.result.y = 240;
    successResponse.result.width = 100;
    successResponse.result.height = 100;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, dummyRequest);
    double expected = 0.0;

    srand(0);  // 最初に識別する色が青ではない乱数シード

    cdcl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // 指定色がNONEの場合、停止することを確認するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunNoneColor)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::NONE;
    double targetDistance = 1000.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    successResponse.result.x = 320;
    successResponse.result.y = 240;
    successResponse.result.width = 100;
    successResponse.result.height = 100;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, dummyRequest);

    double expected = 0.0;

    cdcl.run();  // ライントレースを実行

    // ライントレース後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 正確に終了している
  }

  // targetDistance値が0以下の場合、停止することを確認するテストケース
  TEST(ColorDistanceCameraLineTraceTest, RunMinusDistance)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    COLOR targetColor = COLOR::BLUE;
    double targetDistance = 0.0;
    double targetSpeed = 500.0;
    int targetPoint = 400;
    PidGain gain = { 0.1, 0.05, 0.05 };

    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    successResponse.result.x = 320;
    successResponse.result.y = 240;
    successResponse.result.width = 100;
    successResponse.result.height = 100;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    ColorDistanceCameraLineTrace cdcl(robot, targetColor, targetDistance, targetSpeed, targetPoint,
                                      gain, dummyRequest);
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
