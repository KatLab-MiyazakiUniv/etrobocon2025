/**
 * @file MotorControllerTest.cpp
 * @brief MotorControllerクラスをテストする
 * @author nishijima515
 */

#include "MotorController.h"
#include <gtest/gtest.h>

using namespace spikeapi;

namespace etrobocon2025_test {

  // 右車輪のモータにpower値をセットできるかのテスト
  TEST(MotorControllerTest, SetRightMotorPower)
  {
    MotorController MotorController;
    const int power = 50;
    int initCount = MotorController.getRightMotorCount();
    MotorController.setRightMotorPower(power);
    int currentCount = MotorController.getRightMotorCount();
    EXPECT_LT(initCount, currentCount);
    MotorController.resetRightMotorPower();
  }

  // 右車輪のモータにマイナスのPower値をセットできるかどうかのテスト
  TEST(MotorControllerTest, SetRightMotorMinusPower)
  {
    MotorController MotorController;
    const int power = -50;
    int initCount = MotorController.getRightMotorCount();
    MotorController.setRightMotorPower(power);
    int currentCount = MotorController.getRightMotorCount();
    EXPECT_GT(initCount, currentCount);
    MotorController.resetRightMotorPower();
  }

  // 左車輪のモータにPower値をセットできるかのテスト
  TEST(MotorControllerTest, SetLeftMotorPower)
  {
    MotorController MotorController;
    const int power = 50;
    int initCount = MotorController.getLeftMotorCount();
    MotorController.setLeftMotorPower(power);
    int currentCount = MotorController.getLeftMotorCount();
    EXPECT_LT(initCount, currentCount);
    MotorController.resetLeftMotorPower();
  }

  // 左車輪のモータにマイナスのPower値をセットできるかどうかのテスト
  TEST(MotorControllerTest, SetLeftMotorMinusPower)
  {
    MotorController MotorController;
    const int power = -50;
    int initCount = MotorController.getLeftMotorCount();
    MotorController.setLeftMotorPower(power);
    int currentCount = MotorController.getLeftMotorCount();
    EXPECT_GT(initCount, currentCount);
    MotorController.resetLeftMotorPower();
  }

  // モータに設定するPower値の下限の制限が行われているか確認するテスト
  TEST(MotorControllerTest, SetLeftMotorPowerMin)
  {
    MotorController MotorController;
    const int power = -150;
    MotorController.setLeftMotorPower(MotorControllerClient::MOTOR_POWER_MIN);
    int minCount = MotorController.getLeftMotorPower();
    MotorController.resetLeftMotorPower();
    MotorController.setLeftMotorPower(power);
    int currentCount = MotorController.getLeftMotorPower();
    EXPECT_EQ(minCount, currentCount);
    MotorController.resetLeftMotorPower();
  }

  // モータに設定するPower値の上限の制限が行われているか確認するテスト
  TEST(MotorControllerTest, SetLeftMotorPowerMax)
  {
    MotorController MotorController;
    const int power = 150;
    MotorController.setLeftMotorPower(MotorControllerClient::MOTOR_POWER_MAX);
    int maxCount = MotorController.getLeftMotorPower();
    MotorController.resetLeftMotorPower();
    MotorController.setLeftMotorPower(power);
    int currentCount = MotorController.getLeftMotorPower();
    EXPECT_EQ(maxCount, currentCount);
    MotorController.resetLeftMotorPower();
  }

  // stopWheelsMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, StopWheelsMotor)
  {
    MotorController MotorController;
    int expected = 0;
    MotorController.stopWheelsMotor();
    int actualRight = MotorController.getRightMotorPower();
    EXPECT_EQ(expected, actualRight);
    int actualLeft = MotorController.getLeftMotorPower();
    EXPECT_EQ(expected, actualLeft);
  }

  // brakeWheelsMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, BrakeWheelsMotor)
  {
    MotorController MotorController;
    int expected = 0;
    MotorController.brakeWheelsMotor();
    int actualRight = MotorController.getRightMotorPower();
    EXPECT_EQ(expected, actualRight);
    int actualLeft = MotorController.getLeftMotorPower();
    EXPECT_EQ(expected, actualLeft);
  }

  // アームモータにPower値を設定できるかどうか確認するテスト
  TEST(MotorControllerTest, SetArmMotorPower)
  {
    MotorController MotorController;
    const int power = 50;
    int initCount = MotorController.getArmMotorCount();
    MotorController.setArmMotorPower(power);
    int currentCount = MotorController.getArmMotorCount();
    EXPECT_LT(initCount, currentCount);
    MotorController.resetArmMotorPower();
  }

  // stopArmMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, StopArmMotor)
  {
    MotorController MotorController;
    int expected = 0;
    MotorController.stopArmMotor();
    int actual = MotorController.getArmMotorPower();
    EXPECT_EQ(expected, actual);
  }

  // holdArmMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, HoldArmMotor)
  {
    MotorController MotorController;
    int expected = 0;
    MotorController.holdArmMotor();
    int actual = MotorController.getArmMotorPower();
    EXPECT_EQ(expected, actual);
  }

  // 右タイヤのPower値を取得できるかのテスト
  TEST(MotorControllerTest, GetRightMotorPower)
  {
    MotorController MotorController;
    int expected = 0;
    int actual = MotorController.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    int power = 90;
    MotorController.setRightMotorPower(power);
    expected = 90;
    actual = MotorController.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    power = 200;
    MotorController.setRightMotorPower(power);
    expected = 100;
    actual = MotorController.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    power = -200;
    MotorController.setRightMotorPower(power);
    expected = -100;
    actual = MotorController.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    MotorController.resetRightMotorPower();
  }

  // 左タイヤのPower値を取得できるかのテスト
  TEST(MotorControllerTest, GetleftMotorPower)
  {
    MotorController MotorController;
    int expected = 0;
    int actual = MotorController.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    int power = 80;
    MotorController.setLeftMotorPower(power);
    expected = 80;
    actual = MotorController.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    power = 200;
    MotorController.setLeftMotorPower(power);
    expected = 100;
    actual = MotorController.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    power = -200;
    MotorController.setLeftMotorPower(power);
    expected = -100;
    actual = MotorController.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    MotorController.resetLeftMotorPower();
  }

  // 右車輪のモータに回転速度をセットできるかのテスト
  TEST(MotorControllerTest, SetRightMotorSpeed)
  {
    MotorController MotorController;
    const int speed = 1000;
    int initCount = MotorController.getRightMotorCount();
    MotorController.setRightMotorSpeed(speed);
    int currentCount = MotorController.getRightMotorCount();
    EXPECT_LT(initCount, currentCount);
    MotorController.resetRightMotorPower();
  }

  // 左車輪のモータに回転速度をセットできるかのテスト
  TEST(MotorControllerTest, SetLeftMotorSpeed)
  {
    MotorController MotorController;
    const int speed = 1000;
    int initCount = MotorController.getLeftMotorCount();
    MotorController.setLeftMotorSpeed(speed);
    int currentCount = MotorController.getLeftMotorCount();
    EXPECT_LT(initCount, currentCount);
    MotorController.resetLeftMotorPower();
  }

  // 右車輪のモータの線速度を取得できるかのテスト
  TEST(MotorControllerTest, GetRightMotorSpeed)
  {
    MotorController MotorController;
    double expected = 0.0;
    double actual = MotorController.getRightMotorSpeed();
    EXPECT_EQ(expected, actual);
    double speed = 1000.0;
    MotorController.setRightMotorSpeed(speed);
    double expectedLower = speed - 1;  // getやsetの計算過程でintに丸められた際の許容誤差
    actual = MotorController.getRightMotorSpeed();
    // 　誤差が1以内であれば取得できているとする
    EXPECT_LE(expectedLower, actual);
    EXPECT_GE(speed, actual);
    speed = -1000.0;
    MotorController.setRightMotorSpeed(speed);
    double expectedUpper = speed + 1;  // getやsetの計算過程でintに丸められた際の許容誤差
    actual = MotorController.getRightMotorSpeed();
    // 　誤差が1以内であれば取得できているとする
    EXPECT_GE(actual, speed);
    EXPECT_LE(actual, expectedUpper);
    MotorController.resetRightMotorPower();
  }

  // 左車輪のモータの線速度を取得できるかのテスト
  TEST(MotorControllerTest, GetLeftMotorSpeed)
  {
    MotorController MotorController;
    double expected = 0.0;
    double actual = MotorController.getLeftMotorSpeed();
    EXPECT_EQ(expected, actual);
    double speed = 1000.0;
    MotorController.setLeftMotorSpeed(speed);
    double expectedLower = speed - 1;  // getやsetの計算過程でintに丸められた際の許容誤差
    actual = MotorController.getLeftMotorSpeed();
    // 　誤差が1以内であれば取得できているとする
    EXPECT_LE(expectedLower, actual);
    EXPECT_GE(speed, actual);
    speed = -1000.0;
    MotorController.setLeftMotorSpeed(speed);
    double expectedUpper = speed + 1;  // getやsetの計算過程でintに丸められた際の許容誤差
    actual = MotorController.getLeftMotorSpeed();
    EXPECT_GE(actual, speed);
    EXPECT_LE(actual, expectedUpper);
    MotorController.resetLeftMotorPower();
  }
}  // namespace etrobocon2025_test