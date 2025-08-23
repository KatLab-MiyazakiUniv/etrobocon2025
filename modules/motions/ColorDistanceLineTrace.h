/**
 * @file   ColorDistanceLineTrace.h
 * @brief  指定距離ライントレース動作
 * @author miyahara046 nishijima515
 */

#ifndef COLOR_DISTANCE_LINE_TRACE_H
#define COLOR_DISTANCE_LINE_TRACE_H

#include "LineTrace.h"

class ColorDistanceLineTrace : public LineTrace {
 public:
  /**
   * コンストラクタ
   * @param _targetColor 指定色
   * @param _targetDistance 目標距離 0~
   * @param _targetSpeed 目標速度 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _pidGain PIDゲイン
   */
  ColorDistanceLineTrace(Robot& _robot, COLOR _targetColor, double _targetDistance,
                         double _targetSpeed, int _targetBrightness, const PidGain& _pidGain);

  /**
   * @brief (指定距離まで||指定色認識する)までライントレースする
   */
  using LineTrace::run;

 protected:
  /**
   * @brief 色指定距離ライントレースする際の事前条件判定をする
   * @param targetSpeed 目標速度
   * @note オーバーライド
   */
  bool isMetPreCondition() override;

  /**
   * @brief ライントレースする際の事前処理をする
   * @note オーバーライド必須
   */
  void prepare() override;

  /**
   * @brief 色指定距離ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド
   */
  bool isMetContinuationCondition() override;

 private:
  static constexpr int JUDGE_COUNT = 5;  // 色取得の決定に必要な連続回数
  int colorCount;                        // 色取得した回数
  COLOR targetColor;                     // 指定色
  double targetDistance;                 // 目標距離 0~
  double initDistance;                   // 実行前の走行距離
};

#endif