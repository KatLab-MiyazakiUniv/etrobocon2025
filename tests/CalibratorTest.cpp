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

  // run()において期待した出力がされており，WarningやErrorが出ていないかテスト
  TEST(CalibratorTest, run)
  {
    Robot robot;
    Calibrator calibrator(robot);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    calibrator.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Will Run on the Left Course") != string::npos
                  && output.find("Warning") == string::npos  // Warningがない
                  && output.find("Error") == string::npos;   // Errorがない
    EXPECT_TRUE(actual);
  }
  // waitForStart()において期待した出力がされており，WarningやErrorが出ていないかテスト
  TEST(CalibratorTest, WaitForStart)
  {
    Robot robot;
    Calibrator calibrator(robot);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    calibrator.waitForStart();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("On standby.\n") != string::npos
                  && output.find("Warning") == string::npos  // Warningがない
                  && output.find("Error") == string::npos;   // Errorがない
    EXPECT_TRUE(actual);
  }
  // getIsLeftCourse()により、isLeftCourseの値を取得できるかのテスト
  TEST(CalibratorTest, GetIsLeftCourse)
  {
    Robot robot;
    Calibrator calibrator(robot);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    calibrator.run();
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

  // 目標輝度値を取得できるかのテスト
  TEST(CalibratorTest, getTargetBrightness)
  {
    Robot robot;
    Calibrator calibrator(robot);
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    calibrator.run();
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