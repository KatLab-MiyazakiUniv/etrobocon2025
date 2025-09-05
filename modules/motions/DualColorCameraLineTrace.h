/**
 * @file   DualColorCameraLineTrace.h
 * @brief  色切り替え色距離指定カメラライントレース動作
 * @author nishijima515
 */

#ifndef DUAL_COLOR_CAMERA_LINE_TRACE_H
#define DUAL_COLOR_CAMERA_LINE_TRACE_H

#include "DualCameraPidTracking.h"
#include "ColorJudge.h"

class DualColorCameraLineTrace : public DualCameraPidTracking {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _targetColor 指定色
   * @param _targetDistance 目標距離
   * @param _targetSpeed 目標速度
   * @param _targetXCoordinate 1色目の目標x座標
   * @param _targetXCoordinate2 2色目の目標x座標
   * @param _pidGain PIDゲイン
   * @param _detector 画像処理クラスのポインタ
   */
  DualColorCameraLineTrace(Robot& _robot, COLOR _targetColor, double _targetDistance,
                           double _targetSpeed, int _targetXCoordinate, int _targetXCoordinate2,
                           const PidGain& _pidGain,
                           std::unique_ptr<DualLineBoundingBoxDetector> _detector);

  /**
   * @brief (指定距離まで||指定色認識する)だけカメラライントレースする
   */
  using DualCameraPidTracking::run;

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
  static constexpr int JUDGE_COUNT = 5;                   // 色取得の決定に必要な連続回数
  int colorCount;                                         // 指定色を取得した回数
  COLOR targetColor;                                      // 指定色
  double targetDistance;                                  // 目標距離
  double initDistance;                                    // 実行前の走行距離
  std::unique_ptr<DualLineBoundingBoxDetector> detector;  // 画像処理クラスのポインタ
};

#endif