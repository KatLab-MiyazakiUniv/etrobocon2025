/**
 * @file   MotionDetector.cpp
 * @brief  動体検知用の画像処理クラス
 * @author HaruArima08
 */

#include "MotionDetector.h"
#include <iostream>

MotionDetector::MotionDetector(double _threshold, double _minArea, const cv::Rect& _roi)
  : threshold(_threshold), minArea(_minArea), roi(_roi)
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

  // ROIが画像サイズを超えないようにクリップ
  cv::Rect clippedRoi = roi & cv::Rect(0, 0, frame.cols, frame.rows);
  if(clippedRoi.empty()) {
    std::cerr << "Error: ROI is empty after clipping." << std::endl;
    return;
  }

  // ROI部分を抽出
  cv::Mat roiFrame = frame(clippedRoi);
  cv::Mat roiBgModel = bgModel(clippedRoi);

  // グレースケール化
  cv::Mat gray;
  cv::cvtColor(roiFrame, gray, cv::COLOR_BGR2GRAY);

  // 平滑化
  cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0);
  cv::imwrite("etrobocon2025/datafiles/snapshots/gray.JPEG", gray);

  // 差分計算
  cv::Mat frameDelta;
  cv::absdiff(gray, roiBgModel, frameDelta);
  cv::imwrite("etrobocon2025/datafiles/snapshots/delta.JPEG", frameDelta);

  // 閾値処理と膨張
  cv::Mat thresh;
  cv::threshold(frameDelta, thresh, threshold, 255, cv::THRESH_BINARY);
  cv::imwrite("etrobocon2025/datafiles/snapshots/thresh.JPEG", thresh);
  cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);

  // 輪郭検出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(thresh.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 最大輪郭を見つける
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
    cv::Rect boundingBox = cv::boundingRect(largestContour);

    // ROI内の座標 → 元画像の座標に変換
    boundingBox.x += clippedRoi.x;
    boundingBox.y += clippedRoi.y;

    result.topLeft = boundingBox.tl();
    result.topRight = cv::Point(boundingBox.x + boundingBox.width, boundingBox.y);
    result.bottomLeft = cv::Point(boundingBox.x, boundingBox.y + boundingBox.height);
    result.bottomRight = boundingBox.br();

    // デバッグ用
    cv::Mat debugFrame = frame.clone();
    cv::rectangle(debugFrame, boundingBox, cv::Scalar(0, 255, 0), 2);
    cv::imwrite("etrobocon2025/datafiles/snapshots/debug.JPEG", debugFrame);
  }
}
