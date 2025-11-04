/**
 * @file   DoubleDistanceCameraLineTrace.h
 * @brief  2色指定距離カメラライントレース動作
 * @author miyahara046 HaruArima08
 */

#ifndef DOUBLE_DISTANCE_CAMERA_LINE_TRACE_H
#define DOUBLE_DISTANCE_CAMERA_LINE_TRACE_H

#include "DoubleCameraPidTracking.h"
#include "SocketProtocol.h"

class DoubleDistanceCameraLineTrace : public DoubleCameraPidTracking {
 public:
  /**
   * コンストラクタ
   * @param _targetDistance 目標距離
   * @param _targetSpeed 目標速度
   * @param _targetXCoordinate 目標x座標
   * @param _pidGain PIDゲイン
   * @param _detectionRequest 検出リクエスト
   */
  DoubleDistanceCameraLineTrace(
      Robot& _robot, double _targetDistance, double _targetSpeed, int _targetXCoordinate,
      const PidGain& _pidGain,
      const CameraServer::DoubleBoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief 指定距離だけカメラライントレースする
   */
  using DoubleCameraPidTracking::run;

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
   * @brief
   * 指定距離カメラライントレースする際の継続条件判定をする。
   */
  bool isMetContinuationCondition() override;

 private:
  double targetDistance;                                            // 目標距離
  double initDistance;                                              // 実行前の走行距離
  CameraServer::DoubleBoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
};

#endif