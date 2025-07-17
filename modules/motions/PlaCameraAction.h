/**
 * @file   PlaCameraAction.h
 * @brief  動体検知用動作クラス
 * @author miyahara046
 */

#ifndef PLA_CAMERA_ACTION_H
#define PLA_CAMERA_ACTION_H

#include "SystemInfo.h"
#include "Motion.h"
#include "MotionDetector.h"
#include <vector>
#include <memory>

class PlaCameraAction : public Motion {
 public:
  PlaCameraAction(Robot& _robot, double _threshold, double _minArea, const cv::Rect& _roi);

  void getBackgroundFrame();

  void run() override;

 private:
  bool isMetPreCondition();
  MotionDetector motionDetector;
  std::vector<cv::Mat> capturedFrames;
  size_t numberOfFrames;
  size_t halfNumberOfFrames;
};

#endif
