/**
 * @file   CameraRecoveryActionTest.cpp
 * @brief  CameraRecoveryActionクラスのテスト
 * @author HaruArima08
 */

#include "CameraRecoveryAction.h"
#include <gtest/gtest.h>
#include <iostream>
#include "Robot.h"
#include "DummyCameraCapture.h"
#include "DummyBoundingBoxDetector.h"
#include "SystemInfo.h"

using namespace std;

namespace etrobocon2025_test {
  constexpr double TRANSFORM = 2.0 * WHEEL_RADIUS / TREAD;  // 回頭角度を求める式の係数

  // フレーム取得失敗時、復帰動作が終了するかのテスト
  TEST(CameraRecoveryActionTest, GetFrameFailure)
  {
    int angle = 20;
    double speed = 100.0;
    bool isClockwise = true;
    DummyCameraCapture cameraCapture;
    cameraCapture.setFrameResults({ false });  // getFrame()が失敗するように設定
    Robot robot(cameraCapture);

    auto detector = make_unique<DummyBoundingBoxDetector>();

    CameraRecoveryAction action(robot, angle, speed, isClockwise, move(detector));
    testing::internal::CaptureStdout();                      // 標準出力キャプチャ開始
    action.run();                                            // 復帰動作を実行
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("フレーム取得失敗もしくはフレームが空のため終了\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // 既に検出済みの場合、復帰動作を行わないかのテスト
  TEST(CameraRecoveryActionTest, AlreadyDetected)
  {
    int angle = 20;
    double speed = 100.0;
    bool isClockwise = true;
    DummyCameraCapture cameraCapture;
    cameraCapture.setFrameResults(
        { true, true, true, true, true, true });  // getFrame()が成功するように設定
    Robot robot(cameraCapture);

    auto detector = make_unique<DummyBoundingBoxDetector>();  // デフォルトで常に検出成功

    CameraRecoveryAction action(robot, angle, speed, isClockwise, move(detector));
    testing::internal::CaptureStdout();                      // 標準出力キャプチャ開始
    action.run();                                            // 復帰動作を実行
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("復帰動作は行わない\n") != string::npos;
    EXPECT_TRUE(actual);
  }

  // 復帰動作を行い、検出に成功した場合のテスト
  TEST(CameraRecoveryActionTest, DetectionFailureThenSuccess)
  {
    int angle = 20;
    double speed = 100.0;
    bool isClockwise = true;
    DummyCameraCapture cameraCapture;
    cameraCapture.setFrameResults(
        { true, true, true, true, true, true });  // getFrame()が成功するように設定
    Robot robot(cameraCapture);
    MotorController& motorController = robot.getMotorControllerInstance();

    // 乱数シード1で最初失敗、次に成功するパターンを生成
    auto detector = make_unique<DummyBoundingBoxDetector>(true, 1);

    double expected = angle;  // 指定した回頭角度を期待値とする

    // 実機の setSpeed() 後のモータカウントの増減を模倣するため、ダミークラスで
    // isSetSpeedがtrueのとき、getCount()を呼ぶ度に count += speed× 0.05される。
    // よって、1ループあたりの余分な回転角は speed × 0.05 × TRANSFORM を誤差として見込む。
    double error = speed * 0.05 * TRANSFORM;

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    CameraRecoveryAction action(robot, angle, speed, isClockwise, move(detector));
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool real = output.find("復帰動作完了\n") != string::npos;

    // 回頭後に各モータが回転した角度を計算
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    // 回頭が実行されたことを確認
    EXPECT_LE(expected, actual);
    EXPECT_GE(expected + error, actual);
    // 検出に成功したことを確認
    EXPECT_TRUE(real);
  }

  // 復帰動作を行い、検出に失敗した場合のテスト
  TEST(CameraRecoveryActionTest, DetectionFailureAfterRecovery)
  {
    int angle = 90;
    double speed = 300.0;
    bool isClockwise = true;
    DummyCameraCapture cameraCapture;
    cameraCapture.setFrameResults(
        { true, true, true, true, true, true });  // getFrame()が成功するように設定
    Robot robot(cameraCapture);
    MotorController& motorController = robot.getMotorControllerInstance();

    // 乱数シード2では連続で失敗するパターンを生成
    auto detector = make_unique<DummyBoundingBoxDetector>(true, 2);

    double expected = angle;  // 指定した回頭角度を期待値とする

    // 実機の setSpeed() 後のモータカウントの増減を模倣するため、ダミークラスで
    // isSetSpeedがtrueのとき、getCount()を呼ぶ度に count += speed× 0.05される。
    // よって、1ループあたりの余分な回転角は speed × 0.05 × TRANSFORM を誤差として見込む。
    double error = speed * 0.05 * TRANSFORM;

    // 回頭前のモータカウント
    int initialRightMotorCount = motorController.getRightMotorCount();
    int initialLeftMotorCount = motorController.getLeftMotorCount();

    CameraRecoveryAction action(robot, angle, speed, isClockwise, move(detector));
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    action.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了
    // find("str")はstrが見つからない場合string::nposを返す
    bool real = output.find("復帰動作失敗\n") != string::npos;

    // 回頭後に各モータが回転した角度を計算
    int rightMotorCount = abs(motorController.getRightMotorCount() - initialRightMotorCount);
    int leftMotorCount = abs(motorController.getLeftMotorCount() - initialLeftMotorCount);
    double actual = ((rightMotorCount + leftMotorCount) * TRANSFORM) / 2;

    // 回頭が実行されたことを確認
    EXPECT_LE(expected, actual);
    EXPECT_GE(expected + error, actual);
    // 検出に失敗したことを確認
    EXPECT_TRUE(real);
  }

}  // namespace etrobocon2025_test