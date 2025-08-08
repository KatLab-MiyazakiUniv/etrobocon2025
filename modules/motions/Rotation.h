/**
 * @file   Rotation.h
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#ifndef ROTATION_H
#define ROTATION_H

#include "Motion.h"
#include "Pid.h"
#include "Robot.h"

class Rotation : public Motion {
 public:
  Rotation(Robot& _robot, bool _isClockwise, float _targetAngle, const PidGain& _pidGain);
  virtual ~Rotation() = default;

  void run() override;

 protected:
  virtual void prepare() = 0;
  virtual bool isMetPreCondition() = 0;
  virtual bool isMetContinuationCondition() = 0;

  Robot& robot;
  int leftSign;
  int rightSign;
  float targetAngle;
  PidGain pidGain;
  float goalHeading = 0.0f;
  float currentHeading = 0.0f;
};

#endif