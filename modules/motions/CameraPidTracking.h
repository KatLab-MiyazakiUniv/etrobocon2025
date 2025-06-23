/**
 * @file   CameraPidTracking.h
 * @brief  カメラ画像を使ったPIDライントレースの親クラス
 * @author miyahara046 HaruArima08
 */

#ifndef CAMERA_PID_TRACKING_H
#define CAMERA_PID_TRACKING_H

#include "Motion.h"
#include "BoundingBoxDetector.h"
#include "Pid.h"
#include "SpeedCalculator.h"
#include "Mileage.h"
#include <algorithm>

class CameraPidTracking : public Motion {
 public:
  /**
   * コンストラクタ
   * @brief カメラ画像を使ったPIDライントレースクラスを初期化する
   * @param _robot ロボットインスタンス
   * @param _targetSpeed 目標速度
   * @param _targetPoint 目標点
   * @param _pidGain PIDゲイン
   * @param _boundingBoxDetector 画像処理クラスのポインタ
   * @param _cameraCapture カメラキャプチャクラスのポインタ
   */
  CameraPidTracking(Robot& _robot, double _targetSpeed, int _targetPoint, const PidGain& _pidGain,
                    BoundingBoxDetector& _boundingBoxDetector, CameraCapture& _cameraCapture);

  /**
   * @brief ライントレースを実行する
   */
  void run() override;

 protected:
  /**
   * @brief ライントレースする際の事前条件判定をする
   */
  virtual bool isMetPreCondition() = 0;

  /**
   * @brief ライントレースする際の事前処理をする
   */
  virtual void prepare() = 0;

  /**
   * @brief ライントレースする際の継続条件判定をする。返り値がfalseでモーターが止まる
   */
  virtual bool isMetContinuationCondition() = 0;

 protected:
  BoundingBoxDetector& boundingBoxDetector;  // 画像処理クラスの参照

  // FormatCheck用の改行
  BoundingBoxDetectionResult result;  // バウンディングボックスの座標を格納する構造体
  CameraCapture& cameraCapture;       // カメラキャプチャクラスの参照

  // FormatCheck用の改行
  double targetSpeed;  // 目標速度
  int targetPoint;     // 目標X座標
  PidGain pidGain;     // PIDゲイン
};

#endif