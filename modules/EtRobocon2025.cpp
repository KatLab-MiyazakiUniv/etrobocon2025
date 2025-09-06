/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"
#include <thread>
#include <chrono>

void EtRobocon2025::start()
{
  SocketClient socketClient;  // SocketClientインスタンスを生成
  Robot robot(socketClient);    // RobotインスタンスにSocketClientを渡す

  std::cout << "Hello KATLAB" << std::endl;

  if(robot.getSocketClient().connectToServer()) {
    std::cout << "Connected to server." << std::endl;
  } else {
    std::cout << "Failed to connect to server." << std::endl;
    return;
  };

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

  robot.getSocketClient().disconnectFromServer();
}