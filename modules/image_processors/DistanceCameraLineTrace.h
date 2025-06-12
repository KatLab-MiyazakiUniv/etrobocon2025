/**
 * @file   DistanceLineTrace.h
 * @brief  指定距離ライントレース動作
 * @author miyahara046
 */

#ifndef DISTANCE_CAMERA_LINE_TRACE_H
#define DISTANCE_CAMERA_LINE_TRACE_H

#include "CameraPidTracking.h"

class DistanceCameraLineTrace : public CameraPidTracking {
 public:
  /**
   * コンストラクタ
   * @param _targetDistance 目標距離 0~
   * @param _targetSpeed 目標速度 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _pidGain PIDゲイン
   */
  DistanceCameraLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                          int _targetPoint, const PidGain& _pidGain,
                          ImageProcessor& _imageProcessor);

  /**
   * @brief 指定距離だけライントレースする
   */
  using CameraPidTracking::run;

 protected:
  /**
   * @brief 指定距離ライントレースする際の事前条件判定をする
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */

  bool isMetPreCondition() override;

  /**
   * @brief ライントレースする際の事前処理をする
   * @note オーバーライド必須
   */
  void prepare() override;

  /**
   * @brief 指定距離ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド必須
   */
  bool isMetContinuationCondition() override;

 private:
  double targetDistance;  // 目標距離 0~
  double initDistance;    // 実行前の走行距離
};

#endif