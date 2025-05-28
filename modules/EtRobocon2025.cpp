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

  // DistanceLineTraceのインスタンスを生成
  bool isLeftEdge = true;
  DistanceLineTrace distanceLineTrace(robot,                    // Robotインスタンス
                                      1000.0,                   // 目標距離
                                      800.0,                    // 目標速度
                                      45,                       // 目標輝度
                                      PidGain(0.6, 0.3, 0.05),  // PIDゲイン
                                      isLeftEdge                // エッジ判定
  );
  distanceLineTrace.run();
}
