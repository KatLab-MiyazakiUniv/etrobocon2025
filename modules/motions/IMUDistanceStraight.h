/**
 * @file   IMUDistanceStraight.h
 * @brief  IMU角度補正を用いた目標距離まで直進するクラス
 * @author Hara1274
 */

#ifndef IMU_DISTANCE_STRAIGHT_H
#define IMU_DISTANCE_STRAIGHT_H

#include "Straight.h"
#include "Mileage.h"
#include "Pid.h"

class IMUDistanceStraight : public Straight {
 public:
  /**
   * @brief コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _targetDistance 目標距離 [mm]
   * @param _targetSpeed   目標速度[mm/s]
   * @param _anglePidGain 角度制御PIDゲイン
   */
  IMUDistanceStraight(Robot& _robot, double _targetDistance, double _targetSpeed,
                      const PidGain& _anglePidGain);

  /**
   * @brief 直進する
   */
  void run() override;

 protected:
  /**
   * @brief 直進する際の事前条件判定をする
   */
  bool isMetPreCondition() override;

  /**
   * @brief 直進する際の事前処理をする
   */
  void prepare() override;

  /**
   * @brief 直進する際の動作継続条件判定をする 返り値がtrueの間モーターが回転
   */
  bool isMetContinuationCondition() override;

 private:
  double targetDistance;   // 目標距離 [mm]
  double initialDistance;  // 実行前の走行距離
  Pid anglePid;            // 角度制御PID
  double targetAngle;      // 目標角度
};

#endif
