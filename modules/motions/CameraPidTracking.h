/**
 * @file   CameraPidTracking.h
 * @brief  カメラを使ったPID走行の親クラス
 * @author miyahara046 HaruArima08
 */

#ifndef CAMERA_PID_TRACKING_H
#define CAMERA_PID_TRACKING_H

#include "Motion.h"
#include "BoundingBoxDetector.h"
#include "Pid.h"
#include "SpeedCalculator.h"
#include "Mileage.h"
#include "AngleRotation.h"
#include "RecoveryValidator.h"
#include <algorithm>

class CameraPidTracking : public Motion {
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
  CameraPidTracking(Robot& _robot, double _targetSpeed, int _targetXCoordinate,
                    const PidGain& _pidGain, BoundingBoxDetector& _boundingBoxDetector);

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
  BoundingBoxDetector& boundingBoxDetector;  // 画像処理クラスの参照
  BoundingBoxDetectionResult result;  // バウンディングボックスの座標を格納する構造体
  double targetSpeed;                 // 目標速度
  int targetXCoordinate;              // 目標X座標
  PidGain pidGain;                    // PIDゲイン

 private:
  RecoveryValidator recoveryValidator;  // 復帰動作検証クラス

  /**
   * @brief 検出失敗時の復帰動作を実行する
   * @return true: 復帰成功, false: 復帰失敗
   */
  bool performRecoveryAction();

  /**
   * @brief 角度回頭による復帰を試行する
   * @return true: 復帰成功, false: 復帰失敗
   */
  bool tryRecoveryRotation();
};

#endif
