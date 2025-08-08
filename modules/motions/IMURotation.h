#ifndef IMU_ROTATION_H
#define IMU_ROTATION_H

#include "Rotation.h"

class IMURotation : public Rotation {
 public:
  IMURotation(Robot& _robot, bool _isClockwise, float _targetAngle, const PidGain& _pidGain);

 protected:
  void prepare() override;
  bool isMetPreCondition() override;
  bool isMetContinuationCondition() override;

 private:
  const float tolerance = 1.0f;
};

#endif