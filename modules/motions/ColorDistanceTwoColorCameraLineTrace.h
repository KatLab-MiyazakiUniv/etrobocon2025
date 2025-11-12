/**
 * @file   ColorDistanceTwoColorCameraLineTrace.h
 * @brief  2色色距離指定カメラライントレース動作
 * @author miyahara046 HaruArima08
 */

#ifndef COLOR_DISTANCE_TWO_COLOR_CAMERA_LINE_TRACE_H
#define COLOR_DISTANCE_TWO_COLOR_CAMERA_LINE_TRACE_H

#include "TwoColorCameraPidTracking.h"
#include "ColorJudge.h"
#include "SocketProtocol.h"

class ColorDistanceTwoColorCameraLineTrace : public TwoColorCameraPidTracking {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _targetColor 指定色
   * @param _targetDistance 目標距離
   * @param _targetSpeed 目標速度
   * @param _targetXCoordinate 目標x座標
   * @param _pidGain PIDゲイン
   * @param _detectionRequest 検出リクエスト
   */
  ColorDistanceTwoColorCameraLineTrace(
      Robot& _robot, COLOR _targetColor, double _targetDistance, double _targetSpeed,
      int _targetXCoordinate, const PidGain& _pidGain,
      const CameraServer::TwoColorBoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief (指定距離まで||指定色認識する)だけカメラライントレースする
   */
  using TwoColorCameraPidTracking::run;

 protected:
  /**
   * @brief 色距離指定カメラライントレースする際の事前条件判定をする
   */

  bool isMetPreCondition() override;

  /**
   * @brief ライントレースする際の事前処理をする
   */
  void prepare() override;

  /**
   * @brief
   * 色距離指定カメラライントレースする際の継続条件判定をする。
   */
  bool isMetContinuationCondition() override;

 private:
  static constexpr int JUDGE_COUNT = 2;  // 色取得の決定に必要な連続回数
  int colorCount;                        // 指定色を取得した回数
  COLOR targetColor;                     // 指定色
  double targetDistance;                 // 目標距離
  double initDistance;                   // 実行前の走行距離
  CameraServer::TwoColorBoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
};

#endif