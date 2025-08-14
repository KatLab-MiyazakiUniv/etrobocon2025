/**
 * @file CalibratorTest.cpp
 * @brief Calibratorクラスのテスト
 * @author haruArima08 nishijima515
 */

#include "Calibrator.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

namespace etrobocon2025_test {
  // waitForStart()において期待した出力がされており，WarningやErrorが出ていないかテスト:
  // waitForStartメソッドが正しく動作し、期待される出力がされることを検証する。
  TEST(CalibratorTest, WaitForStart)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    Calibrator calibrator(robot);
    spikeClient.setForceSensorPressed(true);
    testing::internal::CaptureStdout();
    calibrator.waitForStart();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("On standby.\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // 左右ボタンでLRコースを選択できるかのテスト:
  // 左右ボタンの操作によってL/Rコースが正しく選択されることを検証する。
  TEST(CalibratorTest, GetIsLeftCourse)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    Calibrator calibrator(robot);
    // Lコースを選択するシーケンス
    spikeClient.queueButtonPressed(spike::ButtonTarget::LEFT, { true, false });
    spikeClient.queueButtonPressed(spike::ButtonTarget::RIGHT, { true, false });
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    calibrator.selectAndSetCourse();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    bool expected;
    // Leftコースと出力されていた場合
    if(output.find("Will Run on the Left Course") != string::npos) {
      expected = true;  // Lコース
    }
    // Rightコースと出力されていた場合
    else if(output.find("Will Run on the Right Course") != string::npos) {
      expected = false;  // Rコース
    }
    // 想定していない状況
    else {
      expected = NULL;
    }
    bool actual = calibrator.getIsLeftCourse();  // 実際のisLeftCourseを取得
    EXPECT_EQ(expected, actual);                 // 出力とゲッタの値が等しいかテスト
  }

  // 目標輝度値を取得できるかのテスト:
  // 黒と白の輝度測定によって目標輝度値が正しく設定されることを検証する。
  TEST(CalibratorTest, getTargetBrightness)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    Calibrator calibrator(robot);
    // 黒と白の輝度を測定するシーケンス
    spikeClient.queueButtonPressed(spike::ButtonTarget::LEFT, { true, false });
    spikeClient.queueButtonPressed(spike::ButtonTarget::RIGHT, { true, false });
    spikeClient.queueButtonPressed(spike::ButtonTarget::LEFT, { true, false });
    spikeClient.queueButtonPressed(spike::ButtonTarget::RIGHT, { true, false });
    spikeClient.queueReflection({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    calibrator.measureAndSetTargetBrightness();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    string targetString = "Target Brightness Value is ";  // 目標輝度値の直前に書かれている文字列

    // 出力された目標輝度値を取得
    int index = output.find(targetString) + targetString.length();
    string expectedStr = output.substr(index);  // 輝度値を取得（文字列）
    int expected = stoi(expectedStr);           // 文字列を整数値に変換

    int actual = calibrator.getTargetBrightness();  // 実際の輝度値を取得

    EXPECT_EQ(expected, actual);  // 出力とゲッタの値が等しいかテスト
  }
}  // namespace etrobocon2025_test
