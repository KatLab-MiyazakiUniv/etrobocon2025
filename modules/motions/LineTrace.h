
/**
 * @file   LineTrace.h
 * @brief  ライントレース
 * @author miyahara046
 */

#ifndef LINE_TRACE_H
#define LINE_TRACE_H

#include "Motion.h"
#include "Mileage.h"
#include "ColorJudge.h"
#include "Pid.h"
#include "SpeedCalculator.h"
#include "ColorJudge.h"
#include <algorithm>  // max, min を使用するために追加

class LineTrace : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot Robotインスタンス
   * @param _targetSpeed 目標速度 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _pidGain PIDゲイン
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   */
  LineTrace(Robot& _robot, double _targetSpeed, int _targetBrightness, const PidGain& _pidGain,
            bool& _isLeftEdge);

  /**
   * @brief ライントレースする
   */
  void run();

 protected:
  /**
   * @brief ライントレースする際の事前条件判定をする
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */
  virtual bool isMetPreCondition() = 0;

  /**
   * @brief ライントレースする際の事前処理をする
   * @note オーバーライド必須
   */
  virtual void prepare() = 0;

  /**
   * @brief ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド必須
   */
  virtual bool isMetContinuationCondition() = 0;

 protected:
  double targetSpeed;    // 目標速度 0~
  int targetBrightness;  // 目標輝度 0~
  PidGain pidGain;       // PIDゲイン
  bool& isLeftEdge;      // エッジの左右判定(true:左エッジ, false:右エッジ)
};
#endif
