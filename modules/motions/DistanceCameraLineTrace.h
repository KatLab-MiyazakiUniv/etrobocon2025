/**
 * @file   DistanceCameraLineTrace.h
 * @brief  指定距離カメラライントレース動作
 * @author miyahara046 HaruArima08
 */

#ifndef DISTANCE_CAMERA_LINE_TRACE_H
#define DISTANCE_CAMERA_LINE_TRACE_H

#include "CameraPidTracking.h"

class DistanceCameraLineTrace : public CameraPidTracking {
 public:
  /**
   * @brief コンストラクタ
   * @param _targetDistance 目標距離
   * @param _targetSpeed 目標速度
   * @param _targetXCoordinate 目標x座標
   * @param _pidGain PIDゲイン
   * @param _detector 画像処理クラスのポインタ
   */
  DistanceCameraLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                          int _targetXCoordinate, const PidGain& _pidGain,
                          std::unique_ptr<BoundingBoxDetector> _detector);

  /**
   * @brief 指定距離だけカメラライントレースする
   */
  using CameraPidTracking::run;

 protected:
  /**
   * @brief 指定距離カメラライントレースする際の事前条件判定をする
   */

  bool isMetPreCondition() override;

  /**
   * @brief ライントレースする際の事前処理をする
   */
  void prepare() override;

  /**
   * @brief 指定距離カメラライントレースする際の継続条件判定をする。
   */
  bool isMetContinuationCondition() override;

 private:
  double targetDistance;                          // 目標距離
  double initDistance;                            // 実行前の走行距離
  std::unique_ptr<BoundingBoxDetector> detector;  // 画像処理クラスのポインタ
};

#endif