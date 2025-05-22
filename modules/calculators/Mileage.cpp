/**
 **  @file Mileage.cpp
 **  @brief 走行距離を計算するクラス
 **  @author molpui0726
 **/

#include "Mileage.h"
#include "../common/SystemInfo.h"

double Mileage::calculateWheelMileage(int angle)
{
  // タイヤの累計走行距離 ＝ 2 * π * タイヤの半径　* (タイヤの回転角度 / 360[deg])
  return 2.0 * M_PI * RADIUS * static_cast<double>(angle) / 360.0;
}

double Mileage::calculateMileage(int rightAngle, int leftAngle)
{
  // 右タイヤの累計走行距離を計算
  double rightWheelMileage = calculateWheelMileage(rightAngle);
  // 左タイヤの累計走行距離を計算
  double leftWheelMileage = calculateWheelMileage(leftAngle);
  // 走行体全体の累計走行距離 = (右タイヤの累計走行距離 + 左タイヤの累計走行距離) / 2
  return (rightWheelMileage + leftWheelMileage) / 2.0;
}
