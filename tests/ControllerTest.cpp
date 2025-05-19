/**
 * @file ControllerTest.cpp
 * @brief Controllerクラスをテストする
 * @author nishijima515
 */

#include "Controller.h"
#include <gtest/gtest.h>
#include <iostream>

namespace etrobocon2025_test {

  // 右車輪のモータにpower値をセットできるかのテスト
  TEST(ControllerTest, setRightMotorPower)
  {
    // Measurer measurer;
    Controller controller;
    const int power = 50;
    int initCount = controller.getRightCount();
    controller.setRightMotorPower(power);
    int currentCount = controller.getRightCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetRightMotorPower();
  }

  // 右車輪のモータにマイナスのPower値をセットできるかどうかのテスト
  TEST(ControllerTest, setRightMotorMinusPower)
  {
    // Measurer measurer;
    Controller controller;
    const int power = -50;
    int initCount = controller.getRightCount();
    controller.setRightMotorPower(power);
    int currentCount = controller.getRightCount();
    EXPECT_GT(initCount, currentCount);
    controller.resetRightMotorPower();
  }

  // 左車輪のモータにPower値をセットできるかのテスト
  TEST(ControllerTest, setLeftMotorPower)
  {
    // Measurer measurer;
    Controller controller;
    const int power = 50;
    int initCount = controller.getLeftCount();
    controller.setLeftMotorPower(power);
    int currentCount = controller.getLeftCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // 左車輪のモータにマイナスのPower値をセットできるかどうかのテスト
  TEST(ControllerTest, setLeftMotorMinusPower)
  {
    // Measurer measurer;
    Controller controller;
    const int power = -50;
    int initCount = controller.getLeftCount();
    controller.setLeftMotorPower(power);
    int currentCount = controller.getLeftCount();
    EXPECT_GT(initCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // モータに設定するPower値の制限が行われているか確認するテスト
  TEST(ControllerTest, setLeftMotorPowerMin)
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

  // stopWheelsMotor()を呼び出せるか確認するテスト
  TEST(ControllerTest, stopWheelsMotor)
  {
    Controller controller;
    controller.stopWheelsMotor();
    SUCCEED();
  }

  // brakeWheelsMotor()を呼び出せるか確認するテスト
  TEST(ControllerTest, brakeWheelsMotor)
  {
    Controller controller;
    controller.brakeWheelsMotor();
    SUCCEED();
  }

  // アームモータにPower値を設定できるかどうか確認するテスト
  TEST(ControllerTest, setArmMotorPower)
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
  TEST(ControllerTest, stopArmMotor)
  {
    Controller controller;
    controller.stopArmMotor();
    SUCCEED();
  }

  // holdArmMotor()を呼び出せるか確認するテスト
  TEST(ControllerTest, holdArmMotor)
  {
    Controller controller;
    controller.holdArmMotor();
    SUCCEED();
  }

  // 右タイヤのPower値を取得できるかのテスト
  TEST(ControllerTest, getRightMotorPwm)
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
  TEST(ControllerTest, getleftMotorPower)
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
  TEST(ControllerTest, setRightMotorSpeed)
  {
    Controller controller;
    const int speed = 1000;
    int initCount = controller.getRightCount();
    controller.setRightMotorSpeed(speed);
    int currentCount = controller.getRightCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetRightMotorPower();
  }

  // 左車輪のモータに回転速度をセットできるかのテスト
  TEST(ControllerTest, setLeftMotorSpeed)
  {
    Controller controller;
    const int speed = 1000;
    int initCount = controller.getLeftCount();
    controller.setLeftMotorSpeed(speed);
    int currentCount = controller.getLeftCount();
    EXPECT_LT(initCount, currentCount);
    controller.resetLeftMotorPower();
  }

  // 右車輪のモータの回転速度を取得できるかのテスト
  TEST(ControllerTest, getRightMotorSpeed)
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
  TEST(ControllerTest, getLeftMotorSpeed)
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