/**
 * @file   MortorControllerClientTest.cpp
 * @brief  MortorControllerClientクラスのテスト
 * @author takuchi17
 */

#include "gtest/gtest.h"
#include "MotorControllerClient.h"
#include "SpikeClient.h"
#include <cmath>

namespace etrobocon2025_test {

  class MotorControllerClientTest : public ::testing::Test {
   protected:
    SpikeClient dummy_client_;
    MotorControllerClient motor_client_{ dummy_client_ };
  };

  // Power Tests
  // setAndGetRightMotorPowerのテスト: 右モーターのパワーを設定し、正しく取得できることを検証する。
  TEST_F(MotorControllerClientTest, setAndGetRightMotorPower)
  {
    motor_client_.setRightMotorPower(80);
    int expected = 80;
    EXPECT_EQ(expected, motor_client_.getRightMotorPower());
  }

  // setAndGetLeftMotorPowerのテスト: 左モーターのパワーを設定し、正しく取得できることを検証する。
  TEST_F(MotorControllerClientTest, setAndGetLeftMotorPower)
  {
    motor_client_.setLeftMotorPower(-50);
    int expected = -50;
    EXPECT_EQ(expected, motor_client_.getLeftMotorPower());
  }

  // setAndGetArmMotorPowerのテスト:
  // アームモーターのパワーを設定し、正しく取得できることを検証する。
  TEST_F(MotorControllerClientTest, setAndGetArmMotorPower)
  {
    motor_client_.setArmMotorPower(100);
    int expected = 100;
    EXPECT_EQ(expected, motor_client_.getArmMotorPower());
  }

  // resetMotorPowerのテスト: 全てのモーターのパワーをリセットし、0になることを検証する。
  TEST_F(MotorControllerClientTest, resetMotorPower)
  {
    motor_client_.setRightMotorPower(100);
    motor_client_.setLeftMotorPower(100);
    motor_client_.resetWheelsMotorPower();
    EXPECT_EQ(0, motor_client_.getRightMotorPower());
    EXPECT_EQ(0, motor_client_.getLeftMotorPower());

    motor_client_.setArmMotorPower(50);
    motor_client_.resetArmMotorPower();
    EXPECT_EQ(0, motor_client_.getArmMotorPower());
  }

  // Speed Tests
  // setAndGetRightMotorSpeedのテスト: 右モーターの線速度を設定し、正しく取得できることを検証する。
  TEST_F(MotorControllerClientTest, setAndGetRightMotorSpeed)
  {
    double speed = 200.0;
    double expected = speed;
    motor_client_.setRightMotorSpeed(speed);
    EXPECT_NEAR(expected, motor_client_.getRightMotorSpeed(), 1);
  }

  // setAndGetLeftMotorSpeedのテスト: 左モーターの線速度を設定し、正しく取得できることを検証する。
  TEST_F(MotorControllerClientTest, setAndGetLeftMotorSpeed)
  {
    double speed = -200.0;
    double expected = speed;
    motor_client_.setLeftMotorSpeed(speed);
    EXPECT_NEAR(expected, motor_client_.getLeftMotorSpeed(), 1);
  }

  // Stop/Brake/Hold Tests
  // stopAndBrakeWheelsのテスト:
  // 車輪モーターを停止およびブレーキした際に、パワーが0になることを検証する。
  TEST_F(MotorControllerClientTest, stopAndBrakeWheels)
  {
    motor_client_.setRightMotorPower(100);
    motor_client_.setLeftMotorSpeed(100.0);
    motor_client_.stopWheelsMotor();
    EXPECT_EQ(0, motor_client_.getRightMotorPower());
    EXPECT_EQ(0, motor_client_.getLeftMotorPower());

    motor_client_.setRightMotorPower(100);
    motor_client_.brakeWheelsMotor();
    EXPECT_EQ(0, motor_client_.getRightMotorPower());
    EXPECT_EQ(0, motor_client_.getLeftMotorPower());
  }

  // stopAndHoldArmのテスト:
  // アームモーターを停止およびホールドした際に、パワーが0になること、または維持されることを検証する。
  TEST_F(MotorControllerClientTest, stopAndHoldArm)
  {
    motor_client_.setArmMotorPower(100);
    motor_client_.stopArmMotor();
    EXPECT_EQ(0, motor_client_.getArmMotorPower());

    motor_client_.setArmMotorPower(100);
    motor_client_.holdArmMotor();
    EXPECT_EQ(100, motor_client_.getArmMotorPower());
  }

  // Count Tests
  // getMotorCountsのテスト: 各モーターのカウントが正しく取得できることを検証する。
  TEST_F(MotorControllerClientTest, getMotorCounts)
  {
    dummy_client_.setMotorCount(spike::MotorTarget::RIGHT, 1024);
    EXPECT_EQ(1024, motor_client_.getRightMotorCount());

    dummy_client_.setMotorCount(spike::MotorTarget::LEFT, -2048);
    EXPECT_EQ(-2048, motor_client_.getLeftMotorCount());

    dummy_client_.setMotorCount(spike::MotorTarget::ARM, 360);
    EXPECT_EQ(360, motor_client_.getArmMotorCount());
  }

}  // namespace etrobocon2025_test
