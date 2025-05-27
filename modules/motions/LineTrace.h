
/**
 * @file   LineTrace.h
 * @brief  ライントレース
 * @author miyahara046
 */

#ifndef LINE_TRACING_H
#define LINE_TRACING_H

#include "Motion.h"
#include "Mileage.h"
#include "Pid.h"

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
  virtual bool isMetPreCondition(double targetSpeed) = 0;

  /**
   * @brief ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド必須
   */
  virtual bool isMetContinuationCondition() = 0;

 protected:
  double targetSpeed;    // 目標速度 0~
  int targetBrightness;  // 目標輝度 0~
  PidGain pidGain;       // PIDゲイン

  bool& isLeftEdge;         // エッジの左右判定(true:左エッジ, false:右エッジ)
  double initLeftMileage;   // クラス呼び出し時の左車輪の走行距離
  double initRightMileage;  // クラス呼び出し時の右車輪の走行距離
  double initDistance;      // 実行前の走行距離
};

#endif
