/**
 * @file   BottleTwoCatchAction.h
 * @brief  ボトル2つ目のキャッチ動作クラス
 * @author nishijima515
 */

#ifndef BOTTLE_TWO_CATCH_ACTION_H
#define BOTTLE_TWO_CATCH_ACTION_H
#include "SystemInfo.h"
#include "Pid.h"
#include "CompositeMotion.h"
#include "IMUDistanceStraight.h"
#include "UltrasonicDistanceCameraLineTrace.h"
#include "SocketProtocol.h"
#include "PictureColorDistanceStraight.h"
#include "IMUMinAngleRotation.h"
#include <cmath>

class BottleTwoCatchAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _forwardDistance 前進距離[mm]
   * @param _idsSpeed IDSの走行速度[mm/s]
   * @param _ultrasonicDistance 超音波センサーの距離[mm]
   * @param _udclSpeed UDCLの走行速度[mm/s]
   * @param _angle 回頭角度[度]
   * @param _rotatePower 回頭パワー
   * @param _pidGain PIDゲイン
   * @param _detectionRequest 検出リクエスト
   */
  BottleTwoCatchAction(Robot& _robot, double _forwardDistance, double _idsSpeed,
                       double _ultrasonicDistance, double _udclSpeed, double _angle,
                       double _rotatePower, const PidGain& _pidGain,
                       const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief 2本目のボトルのキャッチ動作を行う
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  BoundingBoxDetectionResult result;  // バウンディングボックスの座標を格納する構造体
  double forwardDistance;             // 前進距離[mm]
  double idsSpeed;                    // IDSの走行速度[mm/s]
  double ultrasonicDistance;          // 超音波センサーの距離[mm]
  double udclSpeed;                   // UDCLの走行速度[mm/s]
  double angle;                       // 回頭角度[度]
  double rotatePower;                 // 回頭パワー
  PidGain pidGain;                    // PIDゲイン
};

#endif
