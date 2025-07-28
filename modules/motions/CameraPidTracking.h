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
#include <array>

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
  RecoveryValidator recoveryValidator;                                // 復帰動作検証クラス
  static constexpr std::array<int, 3> RECOVERY_ANGLES = { 3, 6, 9 };  // 復帰回頭角度設定
  static constexpr int RECOVERY_SPEED = 100;                          // 回頭スピード

  /**
   * @brief 検出失敗時の復帰動作
   * @return true: 復帰成功, false: 復帰失敗
   */
  bool performRecoveryAction();

  /**
   * @brief 左右に段階的に角度を広げながら線を探す復帰処理
   * @return true: 検出成功, false: 検出失敗
   */
  bool tryRecoveryRotation();

  /**
   * @brief 指定した方向・角度に回頭して線の検出を試行
   * @param angle 角度
   * @param isClockwise 回頭方向（true: 右回り, false: 左回り）
   * @return true: 検出成功, false: 検出失敗
   */
  bool tryRotationDirection(int angle, bool isClockwise);
};

#endif
