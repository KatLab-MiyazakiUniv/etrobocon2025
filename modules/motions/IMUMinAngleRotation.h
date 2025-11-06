/**
 * @file   IMUMinAngleRotation.h
 * @brief  IMU絶対角度の最小角度で回頭する動作
 * @author Hara127
 */

#ifndef IMU_MIN_ANGLE_ROTATION_H
#define IMU_MIN_ANGLE_ROTATION_H

#include "Rotation.h"
#include "Pid.h"
#include <cmath>

class IMUMinAngleRotation : public Rotation {
 public:
  /**
   * コンストラクタ
   * @param _robot       ロボット制御クラスへの参照
   * @param _targetAngle 目標絶対角度(deg) 0~360
   * @param _basePower   基準パワー値
   * @param _anglePidGain 角度制御用PIDゲイン
   */
  IMUMinAngleRotation(Robot& _robot, int _targetAngle, int _basePower,
                      const PidGain& _anglePidGain);

  /**
   * @brief 回頭動作の事前準備を行う
   */
  void prepare() override;

  /**
   * @brief 回頭する際の事前条件判定をする
   * @return true: 事前条件を満たす, false: 事前条件を満たさない
   */
  bool isMetPreCondition() override;

  /**
   * @brief 回頭する際の継続条件判定をする
   * @return true: 継続, false: 停止（モーター停止）
   */
  bool isMetContinuationCondition() override;

  /**
   * @brief 継続中にモーターを動的制御する
   */
  void updateMotorControl() override;

 private:
  static constexpr float TOLERANCE = 2.0f;  // 許容誤差
  int targetAngle;                          // 目標回転角度(deg) 0~360
  int basePower;                            // 基準パワー値
  Pid anglePid;                             // 角度PID制御クラス
  float currentAngle;                       // 現在の回頭角度
  double angleError;                        // 角度誤差
};

#endif
