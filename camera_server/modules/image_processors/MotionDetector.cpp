/**
 * @file   MotionDetector.cpp
 * @brief  動体検知用の画像処理クラス
 * @author HaruArima08 miyahara046
 */

#include "MotionDetector.h"
#include <iostream>

MotionDetector::MotionDetector(double _threshold, double _minArea, const cv::Rect& _roi)
  : threshold(_threshold), minArea(_minArea), roi(_roi)
{
}

bool MotionDetector::setBackground(const cv::Mat& background)
{
  if(background.empty()) {
    std::cerr << "Error: Invalid background image. It is empty" << std::endl;
    return false;
  }
  bgFrame = background.clone();
  return true;
}

void MotionDetector::detect(const cv::Mat& frame, BoundingBoxDetectionResult& result)
{
  result.wasDetected = false;

  if(bgFrame.empty()) {
    std::cerr << "Error: Background frame is not set. Call setBackground() first." << std::endl;
    return;
  }

  // 一枚目のフレームと二枚目のフレームを比較して最大輪郭（動体）を取得
  std::vector<cv::Point> largestContour = compareTwoFrames(bgFrame, frame);
  if(largestContour.empty()) return;

  // 輪郭からバウンディングボックスを計算し、ROIのオフセットを加える
  result.wasDetected = true;
  cv::Rect boundingBox = cv::boundingRect(largestContour);
  // バウンディングボックスにROIを加算して元画像の左上の点座標を計算
  boundingBox.x += roi.x;
  boundingBox.y += roi.y;

  result.topLeft = boundingBox.tl();
  result.topRight = cv::Point(boundingBox.x + boundingBox.width, boundingBox.y);
  result.bottomLeft = cv::Point(boundingBox.x, boundingBox.y + boundingBox.height);
  result.bottomRight = boundingBox.br();

  // デバッグ用にバウンディングボックスを描画してJPEG出力
  cv::Mat debugFrame = frame.clone();
  cv::rectangle(debugFrame, boundingBox, cv::Scalar(0, 255, 0), 2);
  FrameSave::save(debugFrame, "etrobocon2025/datafiles/snapshots/", "debug.JPEG");
}

std::vector<cv::Point> MotionDetector::compareTwoFrames(const cv::Mat& firstFrame,
                                                        const cv::Mat& secondFrame)
{
  if(firstFrame.empty() || secondFrame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return {};
  }

  // 画像をROIの範囲でクリップ
  cv::Rect clippedRoi = roi & cv::Rect(0, 0, firstFrame.cols, firstFrame.rows);
  if(clippedRoi.empty()) {
    std::cerr << "Error: ROI is empty after clipping." << std::endl;
    return {};
  }
  // １枚目のフレームと２枚目のフレームのROI部分をグレースケール
  cv::Mat roiFirstFrameGray;
  cv::cvtColor(firstFrame(clippedRoi), roiFirstFrameGray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(roiFirstFrameGray, roiFirstFrameGray, cv::Size(21, 21), 0);

  cv::Mat roiSecondFrameGray;
  cv::cvtColor(secondFrame(clippedRoi), roiSecondFrameGray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(roiSecondFrameGray, roiSecondFrameGray, cv::Size(21, 21), 0);

  // 差分画像を計算し、しきい値処理＋膨張で動体領域を強調
  cv::Mat frameDelta;
  cv::absdiff(roiSecondFrameGray, roiFirstFrameGray, frameDelta);
  cv::Mat thresh;
  cv::threshold(frameDelta, thresh, threshold, 255, cv::THRESH_BINARY);
  cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);

  // 輪郭を抽出し、面積が最大かつ最小面積を超えるものを選択
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(thresh.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  double currentLargestArea = 0;
  std::vector<cv::Point> largestContour;
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);
    if(area > minArea && area > currentLargestArea) {
      currentLargestArea = area;
      largestContour = contour;
    }
  }

  return largestContour;
}