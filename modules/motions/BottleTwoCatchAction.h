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
#include "IMUAngleRotation.h"
#include <cmath>

class BottleTwoCatchAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _forwardDistance 前進距離[mm]
   * @param _idsSpeed IDSの走行速度[mm/s]
   * @param _idsPidGain idsのPIDゲイン
   * @param _ultrasonicDistance 超音波センサーの距離[mm]
   * @param _udclSpeed UDCLの走行速度[mm/s]
   * @param _angle 回頭角度[度]
   * @param _rotatePower 回頭パワー
   * @param _udclPidGain udclのPIDゲイン
   * @param _detectionRequest 検出リクエスト
   * @param _roiShrinkValue PCIDS用のROIを狭める値[px] (デフォルト: 0)
   * @param _minimumDistance PictureColorDistanceStraightに設定する最低走行距離[mm]
   * @param _pcidsForwardDistance PCIDS動作用の前進距離[mm]（省略時は_forwardDistanceを使用）
   */
  BottleTwoCatchAction(Robot& _robot, double _forwardDistance, double _idsSpeed,
                       const PidGain& _idsPidGain, double _ultrasonicDistance, double _udclSpeed,
                       double _angle, double _rotatePower, const PidGain& _udclPidGain,
                       const CameraServer::BoundingBoxDetectorRequest& _detectionRequest,
                       int _roiShrinkValue = 0, double _minimumDistance = 0.0,
                       double _pcidsForwardDistance);

  /**
   * @brief 2本目のボトルのキャッチ動作を行う
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  double forwardDistance;                                     // 前進距離[mm]
  double pcidsForwardDistance;                                // PCIDS用前進距離[mm]
  double idsSpeed;                                            // IDSの走行速度[mm/s]
  PidGain idsPidGain;                                         // idsのPIDゲイン
  double ultrasonicDistance;                                  // 超音波センサーの距離[mm]
  double udclSpeed;                                           // UDCLの走行速度[mm/s]
  double angle;                                               // 回頭角度[度]
  double rotatePower;                                         // 回頭パワー
  PidGain udclPidGain;                                        // udclのPIDゲイン
  int roiShrinkValue;                                         // PCIDS用のROIを狭める値[px]
  double minimumDistance;                                     // PCIDSの最低走行距離[mm]
};

#endif
