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
   * コンストラクタ
   * @param _targetDistance 目標距離
   * @param _targetSpeed 目標速度
   * @param _targetPoint 目標x座標
   * @param _pidGain PIDゲイン
   * @param _boundingBoxDetector 画像処理クラスのポインタ
   */
  DistanceCameraLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                          int _targetPoint, const PidGain& _pidGain,
                          BoundingBoxDetector& _boundingBoxDetector);

  /**
   * @brief 指定距離だけカメラライントレースする
   */
  using CameraPidTracking::run;

 protected:
  /**
   * @brief 指定距離カメラライントレースする際の事前条件判定をする
   * @param targetSpeed 目標速度
   */

  bool isMetPreCondition() override;

  /**
   * @brief ライントレースする際の事前処理をする
   */
  void prepare() override;

  /**
   * @brief
   * 指定距離カメラライントレースする際の継続条件判定をする。返り値がfalseでモーターが止まる
   */
  bool isMetContinuationCondition() override;

 private:
  static constexpr int JUDGE_COUNT = 3;  // フレーム取得最大失敗回数
  // format check用
  double targetDistance;  // 目標距離
  double initDistance;    // 実行前の走行距離
  // format check用
  int frameCount;  // フレーム取得カウント
};

#endif