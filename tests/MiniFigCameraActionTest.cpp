/**
 * @file MiniFigCameraActionTest.cpp
 * @brief ミニフィグ撮影動作クラスのテスト
 * @author nishijima515
 */

#include "MiniFigCameraAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "DummyCameraCapture.h"

using namespace std;

namespace etrobocon2025_test {
  // 事前条件判定がfalseで撮影動作を行わない場合のテスト:
  // 事前条件が満たされない場合に、ミニフィグ撮影動作が行われないことを検証する。
  TEST(MiniFigCameraActionTest, NoCameraAction)
  {
    DummyCameraCapture cameraCapture;
    SpikeClient spikeClient;
    Robot robot(spikeClient, cameraCapture);
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
    int position = 1;

    MiniFigCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                               targetRotationSpeed, backTargetDistance, forwardTargetDistance,
                               backSpeed, forwardSpeed, position);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("ミニフィグの撮影動作は行わない。\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // 2回目の撮影でMiniFigの正面の画像を取得する場合のテスト:
  // 2回目の撮影時にミニフィグの正面画像が正しく取得されることを検証する。
  TEST(MiniFigCameraActionTest, PositionIsNotZeroCameraAction)
  {
    DummyCameraCapture cameraCapture;
    SpikeClient spikeClient;
    Robot robot(spikeClient, cameraCapture);
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
    int position = 2;

    MiniFigCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                               targetRotationSpeed, backTargetDistance, forwardTargetDistance,
                               backSpeed, forwardSpeed, position);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("正面での撮影\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // 1回目の撮影でミニフィグが検出出来なかった場合のテスト:
  // ミニフィグが検出されなかった場合に、撮影動作が正しく行われることを検証する。
  TEST(MiniFigCameraActionTest, WasDetectedIsFalseCameraAction)
  {
    DummyCameraCapture cameraCapture;
    SpikeClient spikeClient;
    Robot robot(spikeClient, cameraCapture);
    robot.getMiniFigDirectionResult().wasDetected = false;
    bool isClockwise = false;
    int preTargetAngle = 90;
    int postTargetAngle = 90;
    double targetRotationSpeed = 200;
    double backTargetDistance = 150;
    double forwardTargetDistance = 150;
    double backSpeed = 200;
    double forwardSpeed = 200;
    int position = 3;

    MiniFigCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
                               targetRotationSpeed, backTargetDistance, forwardTargetDistance,
                               backSpeed, forwardSpeed, position);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("ミニフィグ向き判定用写真の撮影\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // position = 0 (1回目の撮影)の場合のテスト
  //   TEST(MiniFigCameraActionTest, PositionIsZeroCameraAction)
  //   {
  //     DummyCameraCapture cameraCapture;
  //     SpikeClient spikeClient;
  //     Robot robot(spikeClient, cameraCapture);
  //     bool isClockwise = false;
  //     int preTargetAngle = 90;
  //     int postTargetAngle = 90;
  //     double targetRotationSpeed = 200;
  //     double backTargetDistance = 150;
  //     double forwardTargetDistance = 150;
  //     double backSpeed = 200;
  //     double forwardSpeed = 200;
  //     int position = 0;

  //     MiniFigCameraAction action(robot, isClockwise, preTargetAngle, postTargetAngle,
  //                                targetRotationSpeed, backTargetDistance, forwardTargetDistance,
  //                                backSpeed, forwardSpeed, position);
  //     testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
  //     action.run();
  //     string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
  //     // find("str")はstrが見つからない場合string::nposを返す
  //     bool actual = output.find("判定動作実施\n") != string::npos;
  //     EXPECT_TRUE(actual);
  //   }
}  // namespace etrobocon2025_test
