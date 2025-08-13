/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"
#include "SpikeClient.h"

// Remove the static Robot instance here, it will be created in start()
// Robot EtRobocon2025::robot;

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;

  // Create a real SpikeClient instance
  SpikeClient spikeClient;

  // Docker containerのIPアドレスを指定して接続（動作確認用）
  if(!spikeClient.connect("172.18.0.2", 8888)) {
    // 実機の際はローカルホストのIPアドレスを指定
    // if(!spikeClient.connect("127.0.0.1", 8888)) {
    std::cerr << "Failed to connect to SpikeServer!" << std::endl;
    return;
  }

  // カメラがない環境で動かす際（動作確認用）
  MockCameraCapture mockCameraCapture;
  Robot robot(spikeClient, mockCameraCapture);
  // 実機の際は以下のようにRobotインスタンスを作成
  // Robot robot(spikeClient);

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
  spikeClient.disconnect();
}