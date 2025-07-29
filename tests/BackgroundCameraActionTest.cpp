/**
 * @file BackgroundCameraActionTest.cpp
 * @brief 風景撮影動作クラスのテスト
 * @author miayahara046
 */

#include "BackgroundCameraAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "DummyCameraCapture.h"
#include "DummyPlaCameraCapture.h"

using namespace std;

namespace etrobocon2025_test {
  // 事前条件判定がfalseで撮影動作を行わない場合のテスト
  TEST(BackgroundCameraActionTest, NoCameraAction)
  {
    DummyPlaCameraCapture dummyPlaCameraCapture;
    dummyPlaCameraCapture.setMotionLikeFrames();

    Robot robot(dummyPlaCameraCapture);
    robot.getBackgroundDirectionResult().wasDetected = true;
    robot.getBackgroundDirectionResult().direction = BackgroundDirection::BACK;
    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200;
    int position = 1;
    cv::Rect roi(0, 0, 800, 600);

    PlaCameraAction plaCameraAction(robot, 30.0, 1000.0, roi);

    BackgroundCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                                  targetRotationSpeed, 30.0, 500.0, roi, position);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("風景の撮影動作は行わない。\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // 2回目の撮影で風景の正面の画像を取得する場合のテスト
  TEST(BackgroundCameraActionTest, PositionIsNotZeroCameraAction)
  {
    DummyPlaCameraCapture dummyPlaCameraCapture;
    dummyPlaCameraCapture.setMotionLikeFrames();

    Robot robot(dummyPlaCameraCapture);
    robot.getBackgroundDirectionResult().wasDetected = true;
    robot.getBackgroundDirectionResult().direction = BackgroundDirection::BACK;
    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200;
    int position = 2;

    cv::Rect roi(0, 0, 800, 600);

    BackgroundCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                                  targetRotationSpeed, 30.0, 500.0, roi, position);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("正面での撮影\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  //   // position = 0(1回目の撮影)の場合のテスト
  //   TEST(BackgroundCameraActionTest, PositionIsZeroCameraAction)
  //   {
  //       DummyPlaCameraCapture dummyPlaCameraCapture;
  //      dummyPlaCameraCapture.setMotionLikeFrames();
  //     Robot robot(dummyPlaCameraCapture);
  //     robot.getBackgroundDirectionResult().wasDetected = true;
  //     robot.getBackgroundDirectionResult().direction = BackgroundDirection::BACK;
  //     bool isClockwise = false;
  //     int preTargetAngle = 90;
  //     int postTargetAngle = 90;
  //     double targetRotationSpeed = 200;
  //     int position = 0;

  //     cv::Rect roi(0, 0, 800, 600);

  //     BackgroundCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
  //                                   targetRotationSpeed, 30.0, 500.0, roi, position);
  //     testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
  //     action.run();
  //     string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
  //     // find("str")はstrが見つからない場合string::nposを返す
  //     bool actual = output.find("判定動作実施\n") != string::npos;
  //     EXPECT_TRUE(actual);
  //   }
}  // namespace etrobocon2025_test
