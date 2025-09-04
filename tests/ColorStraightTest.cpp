/**
 *  @file   ColorStraightTest.cpp
 *  @brief  ColorStraightクラスのテスト
 *  @author HaruArima08
 */

#include "ColorStraight.h"
#include "Mileage.h"
#include <gtest/gtest.h>

using namespace std;

namespace etrobocon2025_test {

  // 最初の色取得で指定色を取得し、停止するテスト
  TEST(ColorStraightTest, RunToGetFirst)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 300.0;
    double basePower = 100.0;
    ColorStraight cs(robot, targetColor, targetSpeed);

    double expected = 0.0;

    // モーターに1回setMotorPower()を呼ぶと、モータカウントが power × 0.05分進む
    // 10ステップ分の走行距離を許容誤差とする
    double error
        = Mileage::calculateMileage(basePower * 0.05 * 10, basePower * 0.05 * 10);  // 許容誤差

    srand(9037);  // 最初に緑を取得する乱数シード

    cs.run();  // 緑まで直進を実行

    // 直進後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GE(actual, expected);
    EXPECT_LE(actual, expected + error);
  }

  // 最初3回の色取得で連続して指定色を取得するテスト
  TEST(ColorStraightTest, RunToGetThreeConsecutive)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 500.0;
    ColorStraight cs(robot, targetColor, targetSpeed);

    double expected = 0.0;

    srand(9000);  // 3回連続して緑を取得する乱数シード

    cs.run();  // 緑までライントレースを実行

    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);  // 初期値より少しは進んでいる
  }

  // 少し直進して指定色を取得するテスト
  TEST(ColorStraightTest, RunToGetLater)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 500.0;
    ColorStraight cs(robot, targetColor, targetSpeed);

    double expectedMinDistance = 10.0;  // 少なくとも10mmは直進する設定

    // 色センサがすぐに緑を返さないような乱数シード
    srand(2131);  // 少し走ってから緑を認識

    cs.run();  // 色取得まで直進

    // 直進後の走行距離を取得
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // ある程度走行してから停止したことを確認（少なくとも10mmは直進している）
    EXPECT_GT(actual, expectedMinDistance);
  }

  // 少し後退して指定色を取得するテストケース
  TEST(ColorStraightTest, RunBackToGetLater)
  {
    Robot robot;
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = -500.0;
    ColorStraight cs(robot, targetColor, targetSpeed);

    double expectedMinDistance = -10.0;  // 少なくとも10mmは後退する設定

    // 色センサがすぐに緑を返さないような乱数シード
    srand(2131);  // 少し走ってから緑を認識

    cs.run();  // 色取得まで後退を実行

    // 後退後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    // ある程度走行してから停止したことを確認（少なくとも10mmは後退している）
    EXPECT_LT(actual, expectedMinDistance);
  }

  // 目標速度が0で停止するかのテスト
  TEST(ColorStraightTest, RunZeroPower)
  {
    Robot robot;
    COLOR targetColor = COLOR::YELLOW;
    double targetSpeed = 0.0;
    ColorStraight cs(robot, targetColor, targetSpeed);

    double expected = 0.0;

    srand(0);  // 最初に黄色を取得しない乱数シード

    cs.run();  // 黄色まで直進を実行

    // 直進後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 直進前後で走行距離に変化はない
  }

  // 目標色がなしで停止するかのテスト
  TEST(ColorStraightTest, RunNoneColor)
  {
    Robot robot;
    COLOR targetColor = COLOR::NONE;
    double targetSpeed = 100.0;
    ColorStraight cs(robot, targetColor, targetSpeed);

    double expected = 0.0;

    cs.run();  // 直進を実行

    // 直進後の走行距離
    int rightCount = robot.getMotorControllerInstance().getRightMotorCount();
    int leftCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expected, actual);  // 直進前後で走行距離に変化はない
  }

}  // namespace etrobocon2025_test