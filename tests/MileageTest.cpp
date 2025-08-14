/**
 * @file MileageTest.cpp
 * @brief 走行距離の測定用クラスのテスト
 * @author molpui0726
 */

#include <cmath>
#include <gtest/gtest.h>
#include "Mileage.h"

namespace etrobocon2025_test {
  // 正の角度での走行距離計算テスト:
  // 正のモーター角度が与えられた場合に、走行距離が正しく計算されることを検証する。
  TEST(MileageTest, CalculateMileagePlus)
  {
    int32_t rightAngle = 30;
    int32_t leftAngle = 40;

    // 計算過程
    // 1.右車輪の累計走行距離を算出
    // double rightWheelMileage = 2.0 * rightAngle * radius * M_PI / 360.0;
    // M_PI = 3.14: rightWheelMileage = 2.0 * 30.0 * 28.0 * 3.14 / 360.0 = 14.6533...
    // M_PI = 3.15: rightWheelMileage = 2.0 * 30.0 * 28.0 * 3.15 / 360.0 = 14.70
    // 2.左車輪の累計走行距離を算出
    // double leftWheelMileage = 2.0 * leftAngle * radius * M_PI / 360.0;
    // M_PI = 3.14: leftWheelMileage = 2.0 * 40.0 * 28.0 * 3.14 / 360.0 = 19.53777...
    // M_PI = 3.15: leftWheelMileage = 2.0 * 40.0 * 28.0 * 3.15 / 360.0 = 19.60
    // 3.両車輪の累計走行距離の平均を算出
    // double expected = (rightWheelMileage + leftWheelMileage) / 2.0;
    // M_PI = 3.14: (14.6533... + 19.5377...) / 2 = 17.09555...
    // M_PI = 3.15: (14.70 + 19.60) / 2 = 17.15
    double expected_min = 17.09555;
    double expected_max = 17.15;

    // actual: 17.104227
    double actual = Mileage::calculateMileage(rightAngle, leftAngle);

    // M_PIが3.14で計算した値よりも大きいアサーションと、3.15で計算した値よりも小さいアサーションを両方満たすか
    EXPECT_LT(expected_min, actual);
    EXPECT_GT(expected_max, actual);
  }

  // 負の角度での走行距離計算テスト:
  // 負のモーター角度が与えられた場合に、走行距離が正しく計算されることを検証する。
  TEST(MileageTest, CalculateMileageMinus)
  {
    int32_t rightAngle = -30;
    int32_t leftAngle = -40;
    double expected_min = -17.15;
    double expected_max = -17.09555;
    double actual = Mileage::calculateMileage(rightAngle, leftAngle);
    EXPECT_LT(expected_min, actual);
    EXPECT_GT(expected_max, actual);
  }

  // ゼロ角度での走行距離計算テスト:
  // モーター角度がゼロの場合に、走行距離がゼロと計算されることを検証する。
  TEST(MileageTest, CalculateMileageZero)
  {
    int32_t rightAngle = 0;
    int32_t leftAngle = 0;
    double expected = 0.0;
    double actual = Mileage::calculateMileage(rightAngle, leftAngle);
    EXPECT_DOUBLE_EQ(expected, actual);
  }
}  // namespace etrobocon2025_test
