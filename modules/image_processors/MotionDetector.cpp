/**
 * @file   MotionDetector.cpp
 * @brief  動体検知用の画像処理クラス
 * @author HaruArima08
 */

#include "MotionDetector.h"
#include <iostream>

MotionDetector::MotionDetector(double _threshold, double _minArea)
  : threshold(_threshold), minArea(_minArea)
{
}

void MotionDetector::setBackground(const cv::Mat& background)
{
  // 背景が空でないこと、グレースケールであることを確認
  if(background.empty() || background.channels() != 1) {
    std::cerr << "Error: Invalid background image. It must be a non-empty, single-channel "
                 "(grayscale) image."
              << std::endl;
    return;
  }
  bgModel = background.clone();
}

void MotionDetector::detect(const cv::Mat& frame, BoundingBoxDetectionResult& result)
{
  result.wasDetected = false;  // 初期状態は検出失敗

  if(frame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return;
  }

  if(bgModel.empty()) {
    std::cerr << "Error: Background model is not set. Call setBackground() first." << std::endl;
    return;
  }

  // フレームをグレースケールに変換
  cv::Mat gray;
  cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

  // 平滑化してノイズを減らす
  cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0);
  //デバッグ用
  cv::imwrite("etrobocon2025/datafiles/snapshots/gray.JPEG", gray);

  // 現在のフレームと静的背景モデルとの差分を計算
  cv::Mat frameDelta;
  cv::absdiff(gray, bgModel, frameDelta);
  // デバッグ用
  cv::imwrite("etrobocon2025/datafiles/snapshots/delta.JPEG", frameDelta);


  // 差分画像を閾値処理して、動きのあった領域を二値化
  cv::Mat thresh;
  cv::threshold(frameDelta, thresh, threshold, 255, cv::THRESH_BINARY);
  // デバッグ用
  cv::imwrite("etrobocon2025/datafiles/snapshots/thresh.JPEG", thresh);

  // 膨張処理で検出領域の穴を埋め、より明確な輪郭を得る
  cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);

  // 輪郭を検出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(thresh.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 最大の輪郭を見つける
  double maxArea = 0;
  std::vector<cv::Point> largestContour;
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);
    if(area > minArea && area > maxArea) {
      maxArea = area;
      largestContour = contour;
    }
  }

  // 動体が見つかった場合
  if(!largestContour.empty()) {
    result.wasDetected = true;

    // 最大輪郭の外接矩形を計算
    cv::Rect boundingBox = cv::boundingRect(largestContour);

    // 矩形の4つの角の座標を結果構造体に格納
    result.topLeft = boundingBox.tl();
    result.topRight = cv::Point(boundingBox.x + boundingBox.width, boundingBox.y);
    result.bottomLeft = cv::Point(boundingBox.x, boundingBox.y + boundingBox.height);
    result.bottomRight = boundingBox.br();

    // デバッグ用に矩形を保存
    cv::Mat debugFrame = frame.clone();
    cv::rectangle(debugFrame, boundingBox, cv::Scalar(0, 255, 0), 2);  // 緑色の枠
    cv::imwrite("etrobocon2025/datafiles/snapshots/debug.JPEG", debugFrame);
  }
}