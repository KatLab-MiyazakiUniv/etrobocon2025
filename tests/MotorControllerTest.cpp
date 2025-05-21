/**
 * @file MotorControllerTest.cpp
 * @brief Controllerクラスをテストする
 * @author nishijima515
 */

#include "MotorController.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {

  // 右車輪のモータにpower値をセットできるかのテスト
  TEST(MotorControllerTest, SetRightMotorPower)
  {
    Controller controller;
    const int power = 50;
    int initCount = controller.getRightMotorCount();
    controller.setRightMotorPower(power);
    int currentCount = controller.getRightMotorCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetRightMotorPower();
  }

  // 右車輪のモータにマイナスのPower値をセットできるかどうかのテスト
  TEST(MotorControllerTest, SetRightMotorMinusPower)
  {
    Controller controller;
    const int power = -50;
    int initCount = controller.getRightMotorCount();
    controller.setRightMotorPower(power);
    int currentCount = controller.getRightMotorCount();
    EXPECT_GT(initCount, currentCount);
    controller.resetRightMotorPower();
  }

  // 左車輪のモータにPower値をセットできるかのテスト
  TEST(MotorControllerTest, SetLeftMotorPower)
  {
    Controller controller;
    const int power = 50;
    int initCount = controller.getLeftMotorCount();
    controller.setLeftMotorPower(power);
    int currentCount = controller.getLeftMotorCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // 左車輪のモータにマイナスのPower値をセットできるかどうかのテスト
  TEST(MotorControllerTest, SetLeftMotorMinusPower)
  {
    Controller controller;
    const int power = -50;
    int initCount = controller.getLeftMotorCount();
    controller.setLeftMotorPower(power);
    int currentCount = controller.getLeftMotorCount();
    EXPECT_GT(initCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // モータに設定するPower値の下限の制限が行われているか確認するテスト
  TEST(MotorControllerTest, SetLeftMotorPowerMin)
  {
    Controller controller;
    const int power = -150;
    controller.setLeftMotorPower(Controller::MOTOR_POWER_MIN);
    int minCount = controller.getLeftMotorPower();
    controller.resetLeftMotorPower();
    controller.setLeftMotorPower(power);
    int currentCount = controller.getLeftMotorPower();
    EXPECT_EQ(minCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // モータに設定するPower値の上限の制限が行われているか確認するテスト
  TEST(MotorControllerTest, SetLeftMotorPowerMax)
  {
    Controller controller;
    const int power = 150;
    controller.setLeftMotorPower(Controller::MOTOR_POWER_MAX);
    int maxCount = controller.getLeftMotorPower();
    controller.resetLeftMotorPower();
    controller.setLeftMotorPower(power);
    int currentCount = controller.getLeftMotorPower();
    EXPECT_EQ(maxCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // stopWheelsMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, StopWheelsMotor)
  {
    Controller controller;
    int expected = 0;
    controller.stopWheelsMotor();
    int actualRight = controller.getRightMotorPower();
    EXPECT_EQ(expected, actualRight);
    int actualLeft = controller.getLeftMotorPower();
    EXPECT_EQ(expected, actualLeft);
  }

  // brakeWheelsMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, BrakeWheelsMotor)
  {
    Controller controller;
    int expected = 0;
    controller.brakeWheelsMotor();
    int actualRight = controller.getRightMotorPower();
    EXPECT_EQ(expected, actualRight);
    int actualLeft = controller.getLeftMotorPower();
    EXPECT_EQ(expected, actualLeft);
  }

  // アームモータにPower値を設定できるかどうか確認するテスト
  TEST(MotorControllerTest, SetArmMotorPower)
  {
    Controller controller;
    const int power = 50;
    int initCount = controller.getArmMotorCount();
    controller.setArmMotorPower(power);
    int currentCount = controller.getArmMotorCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetArmMotorPower();
  }

  // stopArmMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, StopArmMotor)
  {
    Controller controller;
    int expected = 0;
    controller.stopArmMotor();
    int actual = controller.getArmMotorPower();
    EXPECT_EQ(expected, actual);
  }

  // holdArmMotor()を呼び出せるか確認するテスト
  TEST(MotorControllerTest, HoldArmMotor)
  {
    Controller controller;
    int expected = 0;
    controller.holdArmMotor();
    int actual = controller.getArmMotorPower();
    EXPECT_EQ(expected, actual);
  }

  // 右タイヤのPower値を取得できるかのテスト
  TEST(MotorControllerTest, GetRightMotorPower)
  {
    Controller controller;
    int expected = 0;
    int actual = controller.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    int power = 90;
    controller.setRightMotorPower(power);
    expected = 90;
    actual = controller.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    power = 200;
    controller.setRightMotorPower(power);
    expected = 100;
    actual = controller.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    power = -200;
    controller.setRightMotorPower(power);
    expected = -100;
    actual = controller.getRightMotorPower();
    EXPECT_EQ(expected, actual);
    controller.resetRightMotorPower();
  }

  // 左タイヤのPower値を取得できるかのテスト
  TEST(MotorControllerTest, GetleftMotorPower)
  {
    Controller controller;
    int expected = 0;
    int actual = controller.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    int power = 80;
    controller.setLeftMotorPower(power);
    expected = 80;
    actual = controller.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    power = 200;
    controller.setLeftMotorPower(power);
    expected = 100;
    actual = controller.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    power = -200;
    controller.setLeftMotorPower(power);
    expected = -100;
    actual = controller.getLeftMotorPower();
    EXPECT_EQ(expected, actual);
    controller.resetLeftMotorPower();
  }

  // 右車輪のモータに回転速度をセットできるかのテスト
  TEST(MotorControllerTest, SetRightMotorSpeed)
  {
    Controller controller;
    const int speed = 1000;
    int initCount = controller.getRightMotorCount();
    controller.setRightMotorSpeed(speed);
    int currentCount = controller.getRightMotorCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetRightMotorPower();
  }

  // 左車輪のモータに回転速度をセットできるかのテスト
  TEST(MotorControllerTest, SetLeftMotorSpeed)
  {
    Controller controller;
    const int speed = 1000;
    int initCount = controller.getLeftMotorCount();
    controller.setLeftMotorSpeed(speed);
    int currentCount = controller.getLeftMotorCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // 右車輪のモータの回転速度を取得できるかのテスト
  TEST(MotorControllerTest, GetRightMotorSpeed)
  {
    Controller controller;
    int expected = 0;
    int actual = controller.getRightMotorSpeed();
    EXPECT_EQ(expected, actual);
    int speed = 1000;
    controller.setRightMotorSpeed(speed);
    expected = 1000;
    actual = controller.getRightMotorSpeed();
    EXPECT_EQ(expected, actual);
    speed = -1000;
    controller.setRightMotorSpeed(speed);
    expected = -1000;
    actual = controller.getRightMotorSpeed();
    EXPECT_EQ(expected, actual);
    controller.resetRightMotorPower();
  }

  // 左車輪のモータの回転速度を取得できるかのテスト
  TEST(MotorControllerTest, GetLeftMotorSpeed)
  {
    Controller controller;
    int expected = 0;
    int actual = controller.getLeftMotorSpeed();
    EXPECT_EQ(expected, actual);
    int speed = 1000;
    controller.setLeftMotorSpeed(speed);
    expected = 1000;
    actual = controller.getLeftMotorSpeed();
    EXPECT_EQ(expected, actual);
    speed = -1000;
    controller.setLeftMotorSpeed(speed);
    expected = -1000;
    actual = controller.getLeftMotorSpeed();
    EXPECT_EQ(expected, actual);
    controller.resetLeftMotorPower();
  }
}  // namespace etrobocon2025_test