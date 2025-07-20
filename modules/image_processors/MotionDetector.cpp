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
  if(background.empty()) {
    std::cerr << "Error: Invalid background image. It must be a non-empty, single-channel "
                 "(grayscale) image."
              << std::endl;
    return;
  }
  bgFrame = background.clone();
}

std::vector<cv::Point> MotionDetector::compareTwoFrames(const cv::Mat& firstFrame,
                                                        const cv::Mat& secondFrame)
{
  if(firstFrame.empty() || secondFrame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return {};
  }

  // ROIの範囲を画像の範囲にクリップ
  cv::Rect clippedRoi = roi & cv::Rect(0, 0, firstFrame.cols, firstFrame.rows);
  if(clippedRoi.empty()) {
    std::cerr << "Error: ROI is empty after clipping." << std::endl;
    return {};
  }
  // 現在のフレームと比較対象フレームのROI部分をグレースケール
  cv::Mat roiFrame = secondFrame(clippedRoi);
  cv::Mat gray;
  cv::cvtColor(roiFrame, gray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0);

  cv::Mat roiFirstFrameGray;
  cv::cvtColor(firstFrame(clippedRoi), roiFirstFrameGray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(roiFirstFrameGray, roiFirstFrameGray, cv::Size(21, 21), 0);

  // 差分画像を計算し、しきい値処理＋膨張で動体領域を強調
  cv::Mat frameDelta;
  cv::absdiff(gray, roiFirstFrameGray, frameDelta);
  cv::Mat thresh;
  cv::threshold(frameDelta, thresh, threshold, 255, cv::THRESH_BINARY);
  cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);

  // 輪郭を抽出し、面積が最大かつ最小面積を超えるものを選択
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(thresh.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  double maxArea = 0;
  std::vector<cv::Point> largestContour;
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);
    if(area > minArea && area > maxArea) {
      maxArea = area;
      largestContour = contour;
    }
  }

  return largestContour;
}

void MotionDetector::detect(const cv::Mat& frame, BoundingBoxDetectionResult& result)
{
  result.wasDetected = false;

  if(bgFrame.empty()) {
    std::cerr << "Error: Background model is not set. Call setBackground() first." << std::endl;
    return;
  }

  // 背景画像と現在のフレームを比較して最大輪郭（動体）を取得
  std::vector<cv::Point> largestContour = compareTwoFrames(bgFrame, frame);
  if(largestContour.empty()) return;

  // 輪郭からバウンディングボックスを計算し、ROIのオフセットを加える
  result.wasDetected = true;
  cv::Rect boundingBox = cv::boundingRect(largestContour);
  boundingBox.x += roi.x;
  boundingBox.y += roi.y;

  result.topLeft = boundingBox.tl();
  result.topRight = cv::Point(boundingBox.x + boundingBox.width, boundingBox.y);
  result.bottomLeft = cv::Point(boundingBox.x, boundingBox.y + boundingBox.height);
  result.bottomRight = boundingBox.br();

  // デバッグ用にバウンディングボックスを描画してJPEG出力
  cv::Mat debugFrame = frame.clone();
  cv::rectangle(debugFrame, boundingBox, cv::Scalar(0, 255, 0), 2);
  cv::imwrite("etrobocon2025/datafiles/snapshots/debug.JPEG", debugFrame);
}
