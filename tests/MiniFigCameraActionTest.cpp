/**
 * @file MiniFigCameraActionTest.cpp
 * @brief ミニフィグ撮影動作クラスのテスト
 * @author nishijima515
 */

#include "MiniFigCameraAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "MockSocketClient.h"

using namespace std;

namespace etrobocon2025_test {
  // 事前条件判定がfalseで撮影動作を行わない場合のテスト
  TEST(MiniFigCameraActionTest, NoCameraAction)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    robot.getMiniFigDirectionResult().wasDetected = true;
    robot.getMiniFigDirectionResult().direction = MiniFigDirection::BACK;
    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200;
    double backTargetDistance = 150;
    double forwardTargetDistance = 150;
    double backSpeed = 200;
    double forwardSpeed = 200;
    int position = 1;  // isMetPreConditionがfalseになるように設定

    MiniFigCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                               targetRotationSpeed, backTargetDistance, forwardTargetDistance,
                               backSpeed, forwardSpeed, position);
    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("ミニフィグ撮影位置ではありません"), string::npos);
  }

  // 2回目の撮影でMiniFigの正面の画像を取得する場合のテスト
  TEST(MiniFigCameraActionTest, PositionIsNotZeroCameraAction)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // モックのレスポンスを設定
    CameraServer::MiniFigActionResponse dummyResponse;
    dummyResponse.result.wasDetected = true;
    dummyResponse.result.direction = MiniFigDirection::FRONT;
    mockSocketClient.setNextMiniFigResponse(dummyResponse);

    robot.getMiniFigDirectionResult().wasDetected = true;
    int position = 2;
    // isMetPreConditionがtrueになるように設定
    robot.getMiniFigDirectionResult().direction = static_cast<MiniFigDirection>(position);

    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200;
    double backTargetDistance = 150;
    double forwardTargetDistance = 150;
    double backSpeed = 200;
    double forwardSpeed = 200;

    MiniFigCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                               targetRotationSpeed, backTargetDistance, forwardTargetDistance,
                               backSpeed, forwardSpeed, position);
    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("サーバーにミニフィグカメラ撮影を依頼"), string::npos);
    ASSERT_NE(output.find("ミニフィグ撮影結果: 1"), string::npos);
  }

  // 1回目の撮影でミニフィグが検出出来なかった場合のテスト
  TEST(MiniFigCameraActionTest, FirstTimeAndWasDetectedIsFalse)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // モックのレスポンスを設定 (検出失敗)
    CameraServer::MiniFigActionResponse dummyResponse;
    dummyResponse.result.wasDetected = false;
    mockSocketClient.setNextMiniFigResponse(dummyResponse);

    int position = 0;  // 1回目の撮影

    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200;
    double backTargetDistance = 150;
    double forwardTargetDistance = 150;
    double backSpeed = 200;
    double forwardSpeed = 200;

    MiniFigCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                               targetRotationSpeed, backTargetDistance, forwardTargetDistance,
                               backSpeed, forwardSpeed, position);
    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    ASSERT_NE(output.find("サーバーにミニフィグカメラ撮影を依頼"), string::npos);
    ASSERT_NE(output.find("ミニフィグ撮影結果: 0"), string::npos);
    // Robotの状態が更新されたことを確認
    ASSERT_FALSE(robot.getMiniFigDirectionResult().wasDetected);
  }

}  // namespace etrobocon2025_test
