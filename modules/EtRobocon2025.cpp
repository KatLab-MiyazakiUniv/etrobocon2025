/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"
#include "Calibrator.h"

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;
  robot.getCameraCaptureInstance().setCameraID(
      robot.getCameraCaptureInstance().findAvailableCameraID());
  robot.getCameraCaptureInstance().openCamera();
  cv::Mat frame;
  while(!robot.getCameraCaptureInstance().getFrame(frame)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  Calibrator calibrator(robot);
  calibrator.selectAndSetCourse();
  calibrator.measureAndSetTargetBrightness();
  calibrator.waitForStart();
  Area area = Area::DoubleLoop;
  bool isLeftCourse = true;
  int targetBrightness = calibrator.getTargetBrightness();
  AreaMaster areaMaster(robot, area, isLeftCourse, targetBrightness);
  areaMaster.run();
}