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
  robot.getIMUControllerInstance().calculateCorrectionMatrix();


  // IMU手動テスト実行
  std::cout << "=== 手動テスト開始） ===" << std::endl;
  robot.getIMUControllerInstance().resetAngle();
  robot.getIMUControllerInstance().startAngleCalculation();

  while(true) {
    float currentAngle = robot.getIMUControllerInstance().getAngle();
    std::cout << "現在角度: " << currentAngle << " deg" << std::endl;
    if(abs(currentAngle) >= 90.0f) break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  robot.getIMUControllerInstance().stopAngleCalculation();
  std::cout << "=== 手動テスト終了 ===" << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(10000));


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