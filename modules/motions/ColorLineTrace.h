/**
 * @file   ColorLineTrace.h
 * @brief  指定色ライントレース動作
 * @author miyahara046
 */

#ifndef COLOR_LINE_TRACE_H
#define COLOR_LINE_TRACE_H

#include "LineTrace.h"
#include "ColorJudge.h"

class ColorLineTrace : public LineTrace {
 public:
  /**
   * コンストラクタ
   * @param _targetColor 指定色
   * @param _targetSpeed 目標速度
   * @param _targetBrightness 目標輝度 0~
   * @param _gain PIDゲイン
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   */
  ColorLineTrace(Robot& _robot, COLOR _targetColor, double _targetSpeed, int _targetBrightness,
                 const PidGain& _gain, bool& _isLeftEdge);

  /**
   * @brief 指定色までライントレースする
   */
  using LineTrace::run;

 protected:
  /**
   * @brief 指定色ライントレースする際の事前条件判定をする
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */
  bool isMetPreCondition() override;

  /**
   * @brief ライントレースする際の事前処理をする
   * @note オーバーライド必須
   */
  virtual void prepare() override;

  /**
   * @brief 指定色ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド必須
   */
  bool isMetContinuationCondition() override;

 private:
  static constexpr int JUDGE_COUNT = 3;
  int colorCount;
  COLOR targetColor;  // 指定色
};

#endif