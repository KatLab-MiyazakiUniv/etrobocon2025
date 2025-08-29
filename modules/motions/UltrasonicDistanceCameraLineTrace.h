/**
 * @file   UltrasonicDistanceCameraLineTrace.h
 * @brief  超音波距離指定カメラライントレース動作
 * @author nishijima515 miyahara046
 */

#ifndef ULTRASONIC_DISTANCE_CAMERA_LINE_TRACE_H
#define ULTRASONIC_DISTANCE_CAMERA_LINE_TRACE_H

#include "CameraPidTracking.h"
#include "UltraSonic.h"

class UltrasonicDistanceCameraLineTrace : public CameraPidTracking {
 public:
  /**
   * コンストラクタ
   * @param _targetUltrasonicDistance 指定超音波距離
   * @param _targetDistance 目標距離
   * @param _targetSpeed 目標速度
   * @param _targetXCoordinate 目標x座標
   * @param _pidGain PIDゲイン
   * @param _detector 画像処理クラスのポインタ
   */
  UltrasonicDistanceCameraLineTrace(Robot& _robot, double _targetUltrasonicDistance,
                                    double _targetDistance, double _targetSpeed,
                                    int _targetXCoordinate, const PidGain& _pidGain,
                                    std::unique_ptr<BoundingBoxDetector> _detector);

  /**
   * @brief (指定距離まで||指定超音波距離認識する)だけカメラライントレースする
   */
  using CameraPidTracking::run;

 protected:
  /**
   * @brief 超音波距離指定カメラライントレースする際の事前条件判定をする
   */

  bool isMetPreCondition() override;

  /**
   * @brief ライントレースする際の事前処理をする
   */
  void prepare() override;

  /**
   * @brief
   * 超音波距離指定カメラライントレースする際の継続条件判定をする。
   */
  bool isMetContinuationCondition() override;

 private:
  static constexpr int JUDGE_COUNT = 2;  // 超音波取得の決定に必要な連続回数
  int ultrasonicCount;                   // 超音波距離取得した回数
  double targetUltrasonicDistance;       // 指定超音波距離
  double targetDistance;                 // 目標距離
  double initDistance;                   // 実行前の走行距離
  std::unique_ptr<BoundingBoxDetector> detector;  // 画像処理クラスのポインタ
};

#endif