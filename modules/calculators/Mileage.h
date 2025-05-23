/**
 **  @file Mileage.h
 **  @brief 走行距離を計算するクラス
 **  @author molpui0726
 **/

#ifndef MILEAGE_H
#define MILEAGE_H

#include <cmath>
#include "SystemInfo.h"

class Mileage {
 public:
  /**
  ** @brief タイヤの累計走行距離を計算する
  ** @param angle タイヤの回転角度[deg]
  ** @return タイヤの累計走行距離[mm]
  **/
  static double calculateWheelMileage(int32_t angle);

  /**
   ** @brief 走行体全体の累計走行距離を計算する
   ** @param rightAngle 右タイヤの回転角度[deg]
   ** @param leftAngle 左タイヤの回転角度[deg]
   ** @return 走行体全体の累計走行距離[mm]
   **/
  static double calculateMileage(int rightAngle, int leftAngle);

 private:
  Mileage();  // インスタンス化を禁止する
};
#endif
