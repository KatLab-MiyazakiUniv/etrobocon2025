/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;
  robot.getIMUControllerInstance().calculateOffset();
    // IMU手動テスト実行
  robot.getIMUControllerInstance().continuousAngleOutput();

  if(!robot.getCameraCaptureInstance().setCameraID(
         robot.getCameraCaptureInstance().findAvailableCameraID()))
    return;
  if(!robot.getCameraCaptureInstance().openCamera()) return;

  cv::Mat frame;
  while(!robot.getCameraCaptureInstance().getFrame(frame)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  Calibrator calibrator(robot);
  // calibrator.selectAndSetCourse();
  // calibrator.measureAndSetTargetBrightness();

  // bool isLeftCourse = calibrator.getIsLeftCourse();
  // int targetBrightness = calibrator.getTargetBrightness();
  // calibrator.getAngleCheckFrame();
  // calibrator.waitForStart();
  
  Area area = Area::LineTrace;
  bool isLeftCourse = true;
  int targetBrightness = 45;

  AreaMaster lineTraceAreaMaster(robot, area, isLeftCourse, targetBrightness);
  lineTraceAreaMaster.run();
}