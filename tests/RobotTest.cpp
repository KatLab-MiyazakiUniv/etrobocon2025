/**
 * @file RobotTest.cpp
 * @brief Robotクラスのテスト
 * @author takuchi17
 */

#include <gtest/gtest.h>
#include "Robot.h"
#include "SpikeClient.h"
#include "DummyCameraCapture.h"  // Include dummy camera for testing

namespace etrobocon2025_test {

  SpikeClient mockSpikeClient;

  // ゲッターで取得したMotorControllerインスタンスが等しいか確認するテスト
  TEST(RobotTest, GetMotorControllerInstanceReturnsReference)
  {
    Robot robot(mockSpikeClient);  // Pass mock SpikeClient
    MotorControllerClient& motorRef1 = robot.getMotorControllerInstance();
    MotorControllerClient& motorRef2 = robot.getMotorControllerInstance();

    EXPECT_EQ(&motorRef1, &motorRef2);
  }

  // ゲッターで取得したCameraCaptureインスタンスが等しいか確認するテスト
  TEST(RobotTest, GetCameraCaptureInstanceReturnsReference)
  {
    DummyCameraCapture dummyCam;             // Create a dummy camera instance
    Robot robot(mockSpikeClient, dummyCam);  // Pass mock SpikeClient and dummy camera
    ICameraCapture& cameraRef1 = robot.getCameraCaptureInstance();
    ICameraCapture& cameraRef2 = robot.getCameraCaptureInstance();

    EXPECT_EQ(&cameraRef1, &cameraRef2);
  }

  // ゲッターで取得したColorSensorインスタンスが等しいか確認するテスト
  TEST(RobotTest, GetColorSensorInstanceReturnsReference)
  {
    Robot robot(mockSpikeClient);  // Pass mock SpikeClient
    ColorSensorClient& colorSensorRef1 = robot.getColorSensorInstance();
    ColorSensorClient& colorSensorRef2 = robot.getColorSensorInstance();

    EXPECT_EQ(&colorSensorRef1, &colorSensorRef2);
  }

}  // namespace etrobocon2025_test
