/**
 * @file   DistanceLineTrace.h
 * @brief  指定距離ライントレース動作
 * @author miyahara046 nishijima515
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
   */
  DistanceLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                    int _targetBrightness, const PidGain& _pidGain, double _alpha);

  /**
   * コンストラクタ
   * @param _targetDistance 目標距離 0~
   * @param _targetSpeed 目標速度 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _pidGain PIDゲイン
   */
  DistanceLineTrace(Robot& _robot, double _targetDistance, double _targetSpeed,
                    int _targetBrightness, const PidGain& _pidGain);

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
  double targetDistance;  // 目標距離 0~
  double initDistance;    // 実行前の走行距離
};

#endif