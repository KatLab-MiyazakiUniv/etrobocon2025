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
  TEST_F(MotorControllerClientTest, setAndGetRightMotorPower)
  {
    motor_client_.setRightMotorPower(80);
    EXPECT_EQ(motor_client_.getRightMotorPower(), 80);
  }

  TEST_F(MotorControllerClientTest, setAndGetLeftMotorPower)
  {
    motor_client_.setLeftMotorPower(-50);
    EXPECT_EQ(motor_client_.getLeftMotorPower(), -50);
  }

  TEST_F(MotorControllerClientTest, setAndGetArmMotorPower)
  {
    motor_client_.setArmMotorPower(100);
    EXPECT_EQ(motor_client_.getArmMotorPower(), 100);
  }

  TEST_F(MotorControllerClientTest, resetMotorPower)
  {
    motor_client_.setRightMotorPower(100);
    motor_client_.setLeftMotorPower(100);
    motor_client_.resetWheelsMotorPower();
    EXPECT_EQ(motor_client_.getRightMotorPower(), 0);
    EXPECT_EQ(motor_client_.getLeftMotorPower(), 0);

    motor_client_.setArmMotorPower(50);
    motor_client_.resetArmMotorPower();
    EXPECT_EQ(motor_client_.getArmMotorPower(), 0);
  }

  // Speed Tests
  TEST_F(MotorControllerClientTest, setAndGetRightMotorSpeed)
  {
    // speed(mm/s) -> angleSpeed(deg/s)
    // angleSpeed = speed * RAD_TO_DEG / WHEEL_RADIUS
    // angleSpeed = 100.0 * (180.0 / M_PI) / 50.0 = 114.59...
    double speed = 100.0;
    int32_t expected_angle_speed = static_cast<int32_t>(speed * RAD_TO_DEG / WHEEL_RADIUS);
    motor_client_.setRightMotorSpeed(speed);
    EXPECT_EQ(dummy_client_.getMotorSpeed(spike::MotorTarget::RIGHT), expected_angle_speed);
  }

  TEST_F(MotorControllerClientTest, setAndGetLeftMotorSpeed)
  {
    double speed = -200.0;
    int32_t expected_angle_speed = static_cast<int32_t>(speed * RAD_TO_DEG / WHEEL_RADIUS);
    motor_client_.setLeftMotorSpeed(speed);
    EXPECT_EQ(dummy_client_.getMotorSpeed(spike::MotorTarget::LEFT), expected_angle_speed);
  }

  // Stop/Brake/Hold Tests
  TEST_F(MotorControllerClientTest, stopAndBrakeWheels)
  {
    motor_client_.setRightMotorPower(100);
    motor_client_.setLeftMotorSpeed(100.0);
    motor_client_.stopWheelsMotor();
    EXPECT_EQ(motor_client_.getRightMotorPower(), 0);
    EXPECT_EQ(motor_client_.getLeftMotorPower(), 0);

    motor_client_.setRightMotorPower(100);
    motor_client_.brakeWheelsMotor();
    EXPECT_EQ(motor_client_.getRightMotorPower(), 0);
    EXPECT_EQ(motor_client_.getLeftMotorPower(), 0);
  }

  TEST_F(MotorControllerClientTest, stopAndHoldArm)
  {
    motor_client_.setArmMotorPower(100);
    motor_client_.stopArmMotor();
    EXPECT_EQ(motor_client_.getArmMotorPower(), 0);

    motor_client_.setArmMotorPower(100);
    motor_client_.holdArmMotor();
    EXPECT_EQ(motor_client_.getArmMotorPower(), 0);
  }

  // Count Tests
  TEST_F(MotorControllerClientTest, getMotorCounts)
  {
    dummy_client_.setMotorCount(spike::MotorTarget::RIGHT, 1024);
    EXPECT_EQ(motor_client_.getRightMotorCount(), 1024);

    dummy_client_.setMotorCount(spike::MotorTarget::LEFT, -2048);
    EXPECT_EQ(motor_client_.getLeftMotorCount(), -2048);

    dummy_client_.setMotorCount(spike::MotorTarget::ARM, 360);
    EXPECT_EQ(motor_client_.getArmMotorCount(), 360);
  }

}  // namespace etrobocon2025_test
