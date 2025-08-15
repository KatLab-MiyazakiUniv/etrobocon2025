/**
 * @file   EtRobocon2025.h
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#ifndef ETROBOCON2025_H
#define ETROBOCON2025_H

#include <iostream>
#include "Robot.h"
#include "Calibrator.h"
#include "MockCameraCapture.h"

class EtRobocon2025 {
 public:
  /**
   * @brief ETRobocon2025のメイン処理を開始する
   */
  static void start();
};

#endif