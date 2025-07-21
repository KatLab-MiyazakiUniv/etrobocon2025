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

class EtRobocon2025 {
 public:
  static void start();

 private:
  static Robot robot;  // Robotインスタンス
};

#endif