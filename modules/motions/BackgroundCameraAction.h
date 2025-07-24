#ifndef BACKGROUND_CAMERA_ACTION_H
#define BACKGROUND_CAMERA_ACTION_H

#include "PlaCameraAction.h"
#include "BackgroundDirectionDetector.h"
#include "AngleRotation.h"
#include "Robot.h"
#include "FrameSave.h"
#include <thread>
#include <iostream>
#include "CompositeMotion.h"

class BackgroundCameraAction : public CompositeMotion {
 public:
  BackgroundCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                         int _postTargetAngle, double _targetRotationSpeed, double _threshold,
                         double _minArea, const cv::Rect& _roi, int _position);

  void run();

 private:
  bool isMetPreCondition();

  void detectDirection(cv::Mat& frame);

  bool isClockwise = false;
  int preTargetAngle = 90;
  int postTargetAngle = 90;
  double targetRotationSpeed = 200;
  double threshold;
  double minArea;
  const cv::Rect& roi;
  BackgroundDirectionDetector detector;
  int position = 0;
};

#endif
