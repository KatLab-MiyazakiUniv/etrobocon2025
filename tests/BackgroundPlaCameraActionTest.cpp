/**
 * @file BackgroundPlaCameraActionTest.cpp
 * @brief 風景撮影動作クラスのテスト
 * @author miayahara046
 */

#include "BackgroundPlaCameraAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "DummyCameraCapture.h"
#include "DummyPlaCameraCapture.h"

using namespace std;

namespace etrobocon2025_test {
  // 事前条件判定がfalseで撮影動作を行わない場合のテスト
  TEST(BackgroundPlaCameraActionTest, NoCameraAction)
  {
    DummyPlaCameraCapture dummyPlaCameraCapture;
    dummyPlaCameraCapture.setMotionLikeFrames();

    Robot robot(dummyPlaCameraCapture);
    robot.getMotorControllerInstance().resetWheelsMotorPower();

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().initializeOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // 回頭動作用に左回頭状態に設定
    IMUTestControl::rotationStateRef() = -1;

    robot.getBackgroundDirectionResult().wasDetected = true;
    robot.getBackgroundDirectionResult().direction = BackgroundDirection::BACK;
    bool isClockwise = false;
    int preTargetAngle = 10;
    int postTargetAngle = 1;
    int basePower = 50;
    int position = 1;
    cv::Rect roi(0, 0, 800, 600);  // ROI領域を設定

    PlaCameraAction plaCameraAction(robot, 30.0, 1000.0, roi);

    BackgroundPlaCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle, basePower,
                                     30.0, 500.0, roi, position);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("風景の撮影動作は行わない。\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // 2回目の撮影で風景の正面の画像を取得する場合のテスト
  TEST(BackgroundPlaCameraActionTest, PositionIsNotZeroCameraAction)
  {
    DummyPlaCameraCapture dummyPlaCameraCapture;
    dummyPlaCameraCapture.setMotionLikeFrames();

    Robot robot(dummyPlaCameraCapture);
    robot.getMotorControllerInstance().resetWheelsMotorPower();

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().initializeOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // 回頭動作用に左回頭状態に設定
    IMUTestControl::rotationStateRef() = -1;

    robot.getBackgroundDirectionResult().wasDetected = true;
    robot.getBackgroundDirectionResult().direction = BackgroundDirection::BACK;
    bool isClockwise = false;
    int preTargetAngle = 10;
    int postTargetAngle = 1;
    int basePower = 50;
    int position = 2;
    cv::Rect roi(0, 0, 800, 600);  // ROI領域を設定

    BackgroundPlaCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle, basePower,
                                     30.0, 500.0, roi, position);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("正面での撮影\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  //   // position = 0(1回目の撮影)の場合のテスト(モデルがないためコメントアウト)
  //   TEST(BackgroundPlaCameraActionTest, PositionIsZeroCameraAction)
  //   {
  //       DummyPlaCameraCapture dummyPlaCameraCapture;
  //      dummyPlaCameraCapture.setMotionLikeFrames();
  //     Robot robot(dummyPlaCameraCapture);
  //     robot.getBackgroundDirectionResult().wasDetected = true;
  //     robot.getBackgroundDirectionResult().direction = BackgroundDirection::BACK;
  //     bool isClockwise = false;
  //     int preTargetAngle = 90;
  //     int postTargetAngle = 90;
  //     double targetRotationSpeed = 200.0;
  //     int position = 0;
  //     cv::Rect roi(0, 0, 800, 600);  // ROI領域を設定

  //     BackgroundPlaCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
  //                                   targetRotationSpeed, 30.0, 500.0, roi, position);
  //     testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
  //     action.run();
  //     string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
  //     // find("str")はstrが見つからない場合string::nposを返す
  //     bool actual = output.find("判定動作実施\n") != string::npos;
  //     EXPECT_TRUE(actual);
  //   }
}  // namespace etrobocon2025_test
