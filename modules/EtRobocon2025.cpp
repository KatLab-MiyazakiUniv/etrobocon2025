/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"
#include <thread>
#include <chrono>

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;

  Calibrator calibrator(robot);
  calibrator.selectAndSetCourse();
  calibrator.measureAndSetTargetBrightness();
  bool isLeftCourse = calibrator.getIsLeftCourse();
  int targetBrightness = calibrator.getTargetBrightness();
  calibrator.getAngleCheckFrame();
  calibrator.waitForStart();

  Area lineTraceArea = Area::LineTrace;
  AreaMaster lineTraceAreaMaster(robot, lineTraceArea, isLeftCourse, targetBrightness);
  lineTraceAreaMaster.run();
}