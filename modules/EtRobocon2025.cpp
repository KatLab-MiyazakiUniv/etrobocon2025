/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"
#include "Battery.h"

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;

  // バッテリー情報表示
  spikeapi::Battery battery;
  std::cout << "バッテリー電圧: " << battery.getVoltage() << "mV" << std::endl;
  std::cout << "バッテリー電流: " << battery.getCurrent() << "mA" << std::endl;
  robot.getIMUControllerInstance().initializeOffset();
  robot.getIMUControllerInstance().calculateCorrectionMatrix();

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
  calibrator.waitForStart();

  Area lineTraceArea = Area::LineTrace;
  AreaMaster lineTraceAreaMaster(robot, lineTraceArea, true, 52);
  lineTraceAreaMaster.run();
}