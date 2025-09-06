/**
 * @file RobotTest.cpp
 * @brief Robotクラスのテスト
 * @author takuchi17
 */

#include <gtest/gtest.h>
#include "Robot.h"
#include "MockSocketClient.h"

namespace etrobocon2025_test {

  // ゲッターで取得したMotorControllerインスタンスが等しいか確認するテスト
  TEST(RobotTest, GetMotorControllerInstanceReturnsReference)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    MotorController& motorRef1 = robot.getMotorControllerInstance();
    MotorController& motorRef2 = robot.getMotorControllerInstance();

    EXPECT_EQ(&motorRef1, &motorRef2);
  }

  // ゲッターで取得したColorSensorインスタンスが等しいか確認するテスト
  TEST(RobotTest, GetColorSensorInstanceReturnsReference)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);
    spikeapi::ColorSensor& colorSensorRef1 = robot.getColorSensorInstance();
    spikeapi::ColorSensor& colorSensorRef2 = robot.getColorSensorInstance();

    EXPECT_EQ(&colorSensorRef1, &colorSensorRef2);
  }

}  // namespace etrobocon2025_test
