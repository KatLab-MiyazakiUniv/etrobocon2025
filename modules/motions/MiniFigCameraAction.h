#ifndef MINIFIG_CAMERA_ACTION_H
#define MINIFIG_CAMERA_ACTION_H

#include "CompositeMotion.h"

class MiniFigCameraAction : public CompositeMotion {
 public:
  MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle, int _postTargetAngle,
                      double _targetRotationSpeed, double _backTargetDistance,
                      double _forwardTargetDistance, double _backSpeed, double _forwardSpeed,
                      int _position);
  void run() override;

 private:
  bool isMetPreCondition();
  bool isClockwise;
  int preTargetAngle;
  int postTargetAngle;
  double targetRotationSpeed;
  double backTargetDistance;
  double forwardTargetDistance;
  double backSpeed;
  double forwardSpeed;
  int position;
};

#endif