/**
 * @file   DistanceLineTrace.h
 * @brief  指定距離ライントレース動作
 * @author miyahara046
 */

#ifndef DISTANCE_LINE_TRACE_H
#define DISTANCE_LINE_TRACE_H

#include "LineTrace.h"

class DistanceLineTrace : public LineTrace {
 public:
  /**
   * コンストラクタ
   * @param _targetDistance 目標距離 0~
   * @param _targetSpeed 目標速度 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _pidGain PIDゲイン
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   */
  DistanceLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                    int _targetBrightness, const PidGain& _pidGain, bool& _isLeftEdge);

  /**
   * @brief 指定距離だけライントレースする
   */
  using LineTrace::run;

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
  double targetDistance;    // 目標距離 0~
  double initLeftMileage;   // クラス呼び出し時の左車輪の走行距離
  double initRightMileage;  // クラス呼び出し時の右車輪の走行距離
  double initDistance;      // 実行前の走行距離
};

#endif