/**
 * @file   AngleRotation.h
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#ifndef ANGLE_ROTATION_H
#define ANGLE_ROTATION_H

#include "Rotation.h"
#include "Mileage.h"
#include "SystemInfo.h"

class AngleRotation : public Rotation {
 public:
  /**
   * コンストラクタ
   * @param _robot       ロボット制御クラスへの参照
   * @param _targetAngle 目標回転角度(deg) 0~360
   * @param _isClockwise 回頭方向 true:時計回り, false:反時計回り
   * @param _pidGain     PIDゲイン
   */
  AngleRotation(Robot& _robot, int _targetAngle, bool _isClockwise, const PidGain& _pidGain);
  /**
   * @brief 回頭する
   * @note run() メソッドは Rotation クラスの実装をそのまま使用する
   */
  using Rotation::run;

  /**
   * @brief 回頭動作の事前準備を行う
   */
  void prepare() override;

  /**
   * @brief 回頭する際の事前条件判定をする
   * @return true: 事前条件を満たす,false: 事前条件を満たさない
   */
  bool isMetPreCondition() override;

  /**
   * @brief 回頭する際の継続条件判定をする。返り値がfalseでモーターが止まる
   * @return true: 継続, false: 停止（モーター停止）
   */
  bool isMetContinuationCondition() override;

 private:
  double accumulatedAngle = 0.0;                   // 積算角度[deg]
  std::chrono::steady_clock::time_point lastTime;  // 角速度積分の基準時刻（前回の取得時刻）
  double initLeftMileage = 0.0;
  double initRightMileage = 0.0;
  std::chrono::steady_clock::time_point startTime;
  double rotationTime;  // 回転にかける秒数
  double targetAngularVelocity; // 目標角速度 deg/s
};

#endif