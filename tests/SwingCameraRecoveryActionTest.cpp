/**
 * @file   SwingCameraRecoveryActionTest.cpp
 * @brief  SwingCameraRecoveryActionクラスのテスト
 * @author HaruArima08
 */

#include "SwingCameraRecoveryAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "Robot.h"
#include "MockSocketClient.h"
#include "SystemInfo.h"
#include "IMUSetting.h"

using namespace std;

namespace etrobocon2025_test {

  // 既に検出済みの場合、復帰動作を行わないかのテスト
  TEST(SwingCameraRecoveryActionTest, AlreadyDetected)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // モックを設定: 初回検出で成功
    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };
    SwingCameraRecoveryAction action(robot, 20, 100, anglePidGain, 10, dummyRequest);

    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("復帰動作の必要はありません"), string::npos);
  }

  // 復帰動作を行い、再検出で成功した場合のテスト
  TEST(SwingCameraRecoveryActionTest, DetectionSuccessAfterRecovery)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().initializeOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // 絶対角度モードのため角度計算を開始
    IMUSetting imuStart(robot, true);
    imuStart.run();

    // ダミーIMUの回転状態を右回頭に設定
    IMUTestControl::rotationStateRef() = 1;

    // モックを設定: 初回は失敗、2回目は成功
    CameraServer::BoundingBoxDetectorResponse failureResponse;
    failureResponse.result.wasDetected = false;
    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    mockSocketClient.setNextLineDetectionResponse(failureResponse);
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };
    SwingCameraRecoveryAction action(robot, 15, 100, anglePidGain, 10, dummyRequest);

    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("復帰に成功しました。"), string::npos);

    // 角度計算を停止
    IMUSetting imuStop(robot, false);
    imuStop.run();
  }

  // 復帰動作を行い、首振りで成功した場合のテスト
  TEST(SwingCameraRecoveryActionTest, DetectionSuccessAfterSwing)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().initializeOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // 絶対角度モードのため角度計算を開始
    IMUSetting imuStart(robot, true);
    imuStart.run();

    // ダミーIMUの回転状態を右回頭に設定
    IMUTestControl::rotationStateRef() = 1;

    // モックを設定: 初回と再検出は失敗、首振り後に成功
    CameraServer::BoundingBoxDetectorResponse failureResponse;
    failureResponse.result.wasDetected = false;
    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    mockSocketClient.setNextLineDetectionResponse(failureResponse);  // 初回失敗
    mockSocketClient.setNextLineDetectionResponse(failureResponse);  // 再検出失敗
    mockSocketClient.setNextLineDetectionResponse(successResponse);  // 首振り後成功

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };
    SwingCameraRecoveryAction action(robot, 15, 100, anglePidGain, 10, dummyRequest);

    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("首振りで復帰に成功しました。"), string::npos);

    // 角度計算を停止
    IMUSetting imuStop(robot, false);
    imuStop.run();
  }

}  // namespace etrobocon2025_test
