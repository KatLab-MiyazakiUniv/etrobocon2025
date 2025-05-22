/**
 * @file RobotTest.cpp
 * @brief Robotクラスのテスト
 * @author takuchi17
 */

#include <gtest/gtest.h>
#include "Robot.h"

// ゲッターで取得したMotorControllerインスタンスが等しいか確認するテスト
TEST(RobotTest, GetMotorControllerInstanceReturnsReference)
{
  Robot robot;
  MotorController& motorRef1 = robot.getMotorControllerInstance();
  MotorController& motorRef2 = robot.getMotorControllerInstance();

  EXPECT_EQ(&motorRef1, &motorRef2);
}

// ゲッターで取得したCameraCaptureインスタンスが等しいか確認するテスト
TEST(RobotTest, GetCameraCaptureInstanceReturnsReference)
{
  Robot robot;
  CameraCapture& cameraRef1 = robot.getCameraCaptureInstance();
  CameraCapture& cameraRef2 = robot.getCameraCaptureInstance();

  EXPECT_EQ(&cameraRef1, &cameraRef2);
}
