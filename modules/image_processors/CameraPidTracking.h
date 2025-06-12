/**
 * @file   CameraPidTracking.h
 * @brief  カメラ画像を使ったPIDライントレースの親クラス
 * @author miyahara046
 */

#ifndef CAMERA_PID_TRACKING_H
#define CAMERA_PID_TRACKING_H

#include "Motion.h"
#include "ImageProcessor.h"
#include "Pid.h"
#include "SpeedCalculator.h"
#include "Mileage.h"
#include <algorithm>

class CameraPidTracking : public Motion {
 public:
  /**
   * コンストラクタ
   * @brief カメラ画像を使ったPIDライントレースクラスを初期化する
   * @param robot ロボットインスタンス
   * @param targetSpeed 目標速度
   * @param targetPoint 目標点
   * @param pidGain PIDゲイン
   * @param imageProcessor 画像処理クラスのポインタ
   */
  CameraPidTracking(Robot& _robot, double _targetSpeed, int _targetPoint, const PidGain& _pidGain,
                    ImageProcessor& _imageProcessor);

  /**
   * @brief ライントレースを実行する
   */
  void run() override;

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
  double targetSpeed;  // 目標速度
  PidGain pidGain;     // PIDゲイン
  ImageProcessor& imageProcessor;
  int targetPoint;  // 目標X座標
};

#endif