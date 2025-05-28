/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;

  // DistanceLineTrace実験用
  bool isLeftEdge = true;
  DistanceLineTrace distanceLineTrace(robot,                    // Robotインスタンス
                                      3000.0,                   // 目標距離
                                      400.0,                    // 目標速度
                                      45.0,                     // 目標輝度
                                      PidGain(0.5, 0.2, 0.05),  // PIDゲイン
                                      isLeftEdge                // エッジ判定
  );
  distanceLineTrace.run();
}
