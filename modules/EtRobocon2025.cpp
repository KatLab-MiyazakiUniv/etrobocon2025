/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;

  // 背景画像と動体画像を読み込む
  cv::Mat bg = cv::imread("datafiles/snapshots/background.jpg", cv::IMREAD_GRAYSCALE);
  cv::Mat frame = cv::imread("datafiles/snapshots/test_frame.jpg");

  if(bg.empty() || frame.empty()) {
    std::cerr << "Error: 画像の読み込みに失敗しました。" << std::endl;
  }

  // 動体検知器を初期化
  MotionDetector detector(25.0, 500.0);

  // 背景モデルをセット
  detector.setBackground(bg);

  // 動体検知
  BoundingBoxDetectionResult result;
  detector.detect(frame, result);

  if(result.wasDetected) {
    std::cout << "動体検知成功！" << std::endl;
    std::cout << "Top Left: " << result.topLeft << std::endl;
    std::cout << "Top Right: " << result.topRight << std::endl;
    std::cout << "Bottom Left: " << result.bottomLeft << std::endl;
    std::cout << "Bottom Right: " << result.bottomRight << std::endl;
  } else {
    std::cout << "動体は検知されませんでした。" << std::endl;
  }
}
