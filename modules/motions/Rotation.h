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
#include "Mileage.h"
#include "SpeedCalculator.h"

class Rotation : public Motion {
 public:
  Rotation(Robot& _robot, bool _isClockwise, int _targetAngle, const PidGain& _pidGain);
  virtual ~Rotation() = default;

  void run() override;

 protected:
  virtual void prepare() = 0;
  virtual bool isMetPreCondition() = 0;
  virtual bool isMetContinuationCondition() = 0;

  double targetLeftDistance = 0.0;                       // 左モーターの目標移動距離
  double targetRightDistance = 0.0;                      // 右モーターの目標移動距離
  int targetAngle = 0.0;     

  int leftSign;
  int rightSign;
  PidGain pidGain = {0.0, 0.0, 0.0}; // PIDゲインの初期化
};

#endif