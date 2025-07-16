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

  // // 背景画像と動体画像を読み込む
  // cv::Mat bg = cv::imread("etrobocon2025/datafiles/snapshots/bg1.JPEG", cv::IMREAD_GRAYSCALE);
  // cv::Mat frame = cv::imread("etrobocon2025/datafiles/snapshots/m1.JPEG");

  // if(bg.empty() || frame.empty()) {
  //   std::cerr << "Error: 画像の読み込みに失敗しました。" << std::endl;
  // }

  robot.getCameraCaptureInstance().setCameraID(
      robot.getCameraCaptureInstance().findAvailableCameraID());
  robot.getCameraCaptureInstance().openCamera();

  cv::Mat frame;
  while(!robot.getCameraCaptureInstance().getFrame(frame)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  cv::Rect roi(100, 0, 600, 350);

  // 動体検知器を初期化
  PlaCameraAction plaCameraAction(robot, 30.0, 500.0, roi);

  plaCameraAction.run();
}
