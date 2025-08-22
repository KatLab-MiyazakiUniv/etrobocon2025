/**
 * @file UltraSonic.h
 * @brief 超音波センサー制御に用いる関数をまとめたラッパークラス
 * @author nishijima515
 */
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "UltrasonicSensor.h"

class UltraSonic {
 public:
  /**
   * コンストラクタ
   */
  UltraSonic();

  /**
   * @brief 超音波センサーの距離を取得する
   * @return 距離 (mm)
   */
  static double getDistance();

  //   /**
  //    * @brief 超音波センサーの初期化
  //    */
  //   static void init();
 private:
  static spikeapi::UltrasonicSensor ultraSonic;  // 超音波センサーのインスタンス
};

#endif  // ULTRASONIC_H