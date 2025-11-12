/**
 * @file   PictureColorDistanceStraight.h
 * @brief  カメラでの色検出か指定距離で停止する直進動作
 * @author Hara1274
 */

#ifndef PICTURE_COLOR_DISTANCE_STRAIGHT_H
#define PICTURE_COLOR_DISTANCE_STRAIGHT_H

#include "Straight.h"
#include "Pid.h"
#include "SocketProtocol.h"
#include "Mileage.h"
#include "SpeedCalculator.h"

class PictureColorDistanceStraight : public Straight {
 public:
  /**
   * @brief コンストラクタ
   * @param _robot            ロボット本体
   * @param _targetDistance   目標距離[mm]
   * @param _targetSpeed      目標速度[mm/s]
   * @param _anglePidGain     角度補正用PIDゲイン
   * @param _detectionRequest 検出リクエスト（HSV上下限やROIなど）
   */
  PictureColorDistanceStraight(Robot& _robot, double _targetDistance, double _targetSpeed,
                               const PidGain& _anglePidGain,
                               const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief 直進動作を実行する
   */
  void run() override;

 protected:
  /**
   * @brief 直進する際の事前条件判定をする
   */
  bool isMetPreCondition() override;

  /**
   * @brief 直進する際の事前処理をする
   */
  void prepare() override;

  /**
   * @brief 直進する際の動作継続条件判定をする 返り値がtrueの間モーターが回転
   */
  bool isMetContinuationCondition() override;

 private:
  double targetDistance;                                      // 目標距離[mm]
  double initialDistance;                                     // 走行開始時の距離[mm]
  Pid anglePid;                                               // IMU角度補正用PID
  double targetAngle;                                         // 走行開始時の角度
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // カメラ検出設定（HSV上下限など）
};

#endif  // PICTURE_COLOR_DISTANCE_STRAIGHT_H
