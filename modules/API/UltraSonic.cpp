/**
 * @file UltraSonic.cpp
 * @brief 超音波センサー制御に用いる関数をまとめたラッパークラス
 * @author nishijima515
 */

#include "UltraSonic.h"

using namespace spikeapi;

// staticメンバの定義＆初期化（ここで1回だけ）
spikeapi::UltrasonicSensor UltraSonic::ultraSonic(EPort::PORT_F);

UltraSonic::UltraSonic() {}

double UltraSonic::getDistance()
{
  // 超音波センサーの距離を取得
  return static_cast<double>(ultraSonic.getDistance());
}