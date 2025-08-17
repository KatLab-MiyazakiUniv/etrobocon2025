#ifndef BACKGROUND_PLA_CAMERA_ACTION_H
#define BACKGROUND_PLA_CAMERA_ACTION_H

#include "CompositeMotion.h"
#include <opencv2/opencv.hpp>  // For cv::Rect

class BackgroundPlaCameraAction : public CompositeMotion {
 public:
  BackgroundPlaCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                            int _postTargetAngle, double _targetRotationSpeed, double _threshold,
                            double _minArea, const cv::Rect _roi, int _position);
  void run() override;

 private:
  bool isMetPreCondition();
  bool isClockwise;
  int preTargetAngle;
  int postTargetAngle;
  double targetRotationSpeed;
  double threshold;
  double minArea;
  cv::Rect roi;
  int position;
};

#endif