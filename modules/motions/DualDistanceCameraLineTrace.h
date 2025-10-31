/**
 * @file   DualDistanceCameraLineTrace.h
 * @brief  2色指定距離カメラライントレース動作
 * @author miyahara046 HaruArima08
 */

#ifndef DUAL_DISTANCE_CAMERA_LINE_TRACE_H
#define DUAL_DISTANCE_CAMERA_LINE_TRACE_H

#include "DualCameraPidTracking.h"
#include "SocketProtocol.h"

class DualDistanceCameraLineTrace : public DualCameraPidTracking {
 public:
  /**
   * コンストラクタ
   * @param _targetDistance 目標距離
   * @param _targetSpeed 目標速度
   * @param _targetXCoordinate 目標x座標
   * @param _pidGain PIDゲイン
   * @param _detectionRequestFirst 検出リクエスト
   * @param _detectionRequestSecond 検出リクエスト
   */
  DualDistanceCameraLineTrace(
      Robot& _robot, double _targetDistance, double _targetSpeed, int _targetXCoordinate,
      const PidGain& _pidGain,
      const CameraServer::BoundingBoxDetectorRequest& _detectionRequestFirst,
      const CameraServer::BoundingBoxDetectorRequest& _detectionRequestSecond);

  /**
   * @brief 指定距離だけカメラライントレースする
   */
  using DualCameraPidTracking::run;

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
  CameraServer::BoundingBoxDetectorRequest detectionRequestFirst;   // 検出リクエスト
  CameraServer::BoundingBoxDetectorRequest detectionRequestSecond;  // 検出リクエスト
};

#endif