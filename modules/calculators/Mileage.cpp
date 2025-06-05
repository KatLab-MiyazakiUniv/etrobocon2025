/**
 **  @file Mileage.cpp
 **  @brief 走行距離を計算するクラス
 **  @author molpui0726
 **/

#include "Mileage.h"

// MotorController でモーターの角位置を int32_t 型で取得しているため、
// double ではなく int32_t 型を使用する
double Mileage::calculateWheelMileage(int32_t angle)
{
  // タイヤの累計走行距離 ＝ 2 * π * タイヤの半径　* (タイヤの回転角度 / 360[deg])
  return 2.0 * PI * WHEEL_RADIUS * static_cast<double>(angle) / 360.0;
}

double Mileage::calculateMileage(int32_t rightAngle, int32_t leftAngle)
{
  // 右タイヤの累計走行距離を計算
  double rightWheelMileage = calculateWheelMileage(rightAngle);
  // 左タイヤの累計走行距離を計算
  double leftWheelMileage = calculateWheelMileage(leftAngle);
  // 走行体全体の累計走行距離 = (右タイヤの累計走行距離 + 左タイヤの累計走行距離) / 2
  return (rightWheelMileage + leftWheelMileage) / 2.0;
}
