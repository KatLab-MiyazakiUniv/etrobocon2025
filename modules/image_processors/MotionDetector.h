/**
 * @file   MotionDetector.h
 * @brief  動体検知用の画像処理クラス
 * @author HaruArima08 miyahara046
 */

#ifndef MOTION_DETECTOR_H
#define MOTION_DETECTOR_H

#include "BoundingBoxDetector.h"
#include <opencv2/opencv.hpp>

class MotionDetector : public BoundingBoxDetector {
 public:
  MotionDetector(double _threshold, double _minArea, const cv::Rect& _roi);

  void setBackground(const cv::Mat& background);

  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) override;

  std::vector<cv::Point> compareTwoFrames(const cv::Mat& firstFrame, const cv::Mat& secondFrame);

 private:
  double threshold;
  double minArea;
  cv::Rect roi;
  cv::Mat bgFrame;
};

#endif
