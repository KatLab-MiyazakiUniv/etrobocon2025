/**
 * @file   DualCameraPidTracking.h
 * @brief  カメラを使った色切り替えPID走行の親クラス
 * @author miyahara046 HaruArima08
 */

#ifndef DUAL_CAMERA_PID_TRACKING_H
#define DUAL_CAMERA_PID_TRACKING_H

#include "Motion.h"
#include "DualLineBoundingBoxDetector.h"
#include "Pid.h"
#include "SpeedCalculator.h"
#include "Mileage.h"
#include <algorithm>

class DualCameraPidTracking : public Motion {
 public:
  /**
   * コンストラクタ
   * @brief カメラ画像を使ったPID走行クラスを初期化する
   * @param _robot ロボットインスタンス
   * @param _targetSpeed 目標速度
   * @param _targetXCoordinate 目標x座標
   * @param _pidGain PIDゲイン
   * @param _boundingBoxDetector 画像処理クラスのポインタ
   */
  DualCameraPidTracking(Robot& _robot, double _targetSpeed, int _targetXCoordinate,
                        int _targetXCoordinate2, const PidGain& _pidGain,
                        DualLineBoundingBoxDetector& _boundingBoxDetector);

  /**
   * @brief カメラ走行を実行する
   */
  void run() override;

 protected:
  /**
   * @brief カメラ走行する際の事前条件判定をする
   */
  virtual bool isMetPreCondition() = 0;

  /**
   * @brief カメラ走行する際の事前処理をする
   */
  virtual void prepare() = 0;

  /**
   * @brief カメラ走行する際の継続条件判定をする。返り値がfalseでモーターが止まる
   */
  virtual bool isMetContinuationCondition() = 0;

 protected:
  DualLineBoundingBoxDetector& boundingBoxDetector;  // 画像処理クラスの参照
  BoundingBoxDetectionResult result;  // バウンディングボックスの座標を格納する構造体
  double targetSpeed;                 // 目標速度
  int targetXCoordinate;              // 目標X座標
  int targetXCoordinate2;             // 2色目の目標X座標
  PidGain pidGain;                    // PIDゲイン
};

#endif