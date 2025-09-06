/**
 * @file BackgroundPlaCameraActionTest.cpp
 * @brief 風景撮影動作クラスのテスト
 * @author miayahara046
 */

#include "BackgroundPlaCameraAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "MockSocketClient.h"

using namespace std;

namespace etrobocon2025_test {
  // 事前条件判定がfalseで撮影動作を行わない場合のテスト
  TEST(BackgroundPlaCameraActionTest, NoCameraAction)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    robot.getBackgroundDirectionResult().wasDetected = true;
    robot.getBackgroundDirectionResult().direction = BackgroundDirection::BACK;
    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200.0;
    int position = 1;  // isMetPreConditionがfalseになるように設定
    cv::Rect roi(0, 0, 800, 600);

    BackgroundPlaCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                                     targetRotationSpeed, 30.0, 500.0, roi, position);
    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    // isMetPreConditionがfalseの時に特定の文字列が出力されるかチェック
    ASSERT_NE(output.find("プラレール撮影位置ではありません"), string::npos);
  }

  // 2回目の撮影で風景の正面の画像を取得する場合のテスト
  TEST(BackgroundPlaCameraActionTest, PositionIsNotZeroCameraAction)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    // モックのレスポンスを設定
    CameraServer::BackgroundPlaActionResponse dummyResponse;
    dummyResponse.result.wasDetected = true;
    dummyResponse.result.direction = BackgroundDirection::FRONT;
    mockSocketClient.setNextBackgroundPlaResponse(dummyResponse);

    robot.getBackgroundDirectionResult().wasDetected = true;
    int position = 2;
    // isMetPreConditionがtrueになるように設定
    robot.getBackgroundDirectionResult().direction = static_cast<BackgroundDirection>(position);

    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200.0;
    cv::Rect roi(0, 0, 800, 600);

    BackgroundPlaCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                                     targetRotationSpeed, 30.0, 500.0, roi, position);
    testing::internal::CaptureStdout();
    action.run();
    string output = testing::internal::GetCapturedStdout();
    // runが実行され、特定の文字列が出力されるかチェック
    ASSERT_NE(output.find("サーバーに風景・プラレールカメラ撮影を依頼"), string::npos);
    ASSERT_NE(output.find("風景・プラレール撮影結果: 1"), string::npos);
  }

}  // namespace etrobocon2025_test
