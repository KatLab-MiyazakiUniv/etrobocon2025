/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"
#include "SpikeClient.h"  // Include the real SpikeClient

// Remove the static Robot instance here, it will be created in start()
// Robot EtRobocon2025::robot;

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;

  // Create a real SpikeClient instance
  SpikeClient realSpikeClient;
  // Connect to the SpikeServer (assuming localhost:8888)
  if(!realSpikeClient.connect("172.18.0.5", 8888)) {
    // if (!realSpikeClient.connect("127.0.0.1", 8888)) {
    std::cerr << "Failed to connect to SpikeServer!" << std::endl;
    return;
  }

  // Pass the real SpikeClient to the Robot constructor
  // Robot robot(realSpikeClient);
  // If you need to use a camera, you can create a MockCameraCapture or use the real one
  MockCameraCapture mockCameraCapture;              // Use MockCameraCapture for testing
  Robot robot(realSpikeClient, mockCameraCapture);  // Use MockCameraCapture for testing

  if(!robot.getCameraCaptureInstance().setCameraID(
         robot.getCameraCaptureInstance().findAvailableCameraID()))
    return;
  if(!robot.getCameraCaptureInstance().openCamera()) return;

  cv::Mat frame;
  while(!robot.getCameraCaptureInstance().getFrame(frame)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  Calibrator calibrator(robot);
  calibrator.selectAndSetCourse();
  calibrator.measureAndSetTargetBrightness();
  bool isLeftCourse = calibrator.getIsLeftCourse();
  int targetBrightness = calibrator.getTargetBrightness();
  calibrator.getAngleCheckFrame();
  calibrator.waitForStart();

  Area lineTraceArea = Area::LineTrace;
  AreaMaster lineTraceAreaMaster(robot, lineTraceArea, true, 50);
  lineTraceAreaMaster.run();

  // Disconnect the SpikeClient when done
  realSpikeClient.disconnect();
}