/**
 * @file   CameraRecoveryActionTest.cpp
 * @brief  CameraRecoveryActionクラスのテスト
 * @author HaruArima08
 */

#include "CameraRecoveryAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "Robot.h"
#include "MockSocketClient.h"
#include "SystemInfo.h"

using namespace std;

namespace etrobocon2025_test {

  // 既に検出済みの場合、復帰動作を行わないかのテスト
  TEST(CameraRecoveryActionTest, AlreadyDetected)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // モックを設定: 初回検出で成功
    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    CameraRecoveryAction action(robot, 20, 100.0, true, dummyRequest);

    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("復帰動作の必要はありません"), string::npos);
  }

  // 復帰動作を行い、再検出で成功した場合のテスト
  TEST(CameraRecoveryActionTest, DetectionSuccessAfterRecovery)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // モックを設定: 初回は失敗、2回目は成功
    CameraServer::BoundingBoxDetectorResponse failureResponse;
    failureResponse.result.wasDetected = false;
    CameraServer::BoundingBoxDetectorResponse successResponse;
    successResponse.result.wasDetected = true;
    mockSocketClient.setNextLineDetectionResponse(failureResponse);
    mockSocketClient.setNextLineDetectionResponse(successResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    CameraRecoveryAction action(robot, 20, 100.0, true, dummyRequest);

    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("復帰に成功しました。"), string::npos);
  }

  // 復帰動作を行い、再検出でも失敗した場合のテスト
  TEST(CameraRecoveryActionTest, DetectionFailureAfterRecovery)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // モックを設定: 2回とも失敗
    CameraServer::BoundingBoxDetectorResponse failureResponse;
    failureResponse.result.wasDetected = false;
    mockSocketClient.setNextLineDetectionResponse(failureResponse);
    mockSocketClient.setNextLineDetectionResponse(failureResponse);

    CameraServer::BoundingBoxDetectorRequest dummyRequest;
    CameraRecoveryAction action(robot, 90, 300.0, true, dummyRequest);

    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("復帰できませんでした"), string::npos);
  }

}  // namespace etrobocon2025_test
