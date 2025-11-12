/**
 * @file   CameraRecoveryAction.h
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#ifndef CAMERA_RECOVERY_ACTION_H
#define CAMERA_RECOVERY_ACTION_H

#include "CompositeMotion.h"
#include "IMUAngleRotation.h"
#include "SocketProtocol.h"
#include "Snapshot.h"
#include <iostream>
#include <cmath>
#include "Pid.h"

class CameraRecoveryAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _lineDirectionAngle ラインの方向角度（絶対角度, deg） 0~360
   * @param _basePower 基準パワー値
   * @param _anglePidGain 角度制御用PIDゲイン
   * @param _swingAngle 首振り角度（deg）
   * @param _detectionRequest 検出リクエスト
   */
  CameraRecoveryAction(Robot& _robot, int _lineDirectionAngle, int _basePower,
                       const PidGain& _anglePidGain, int _swingAngle,
                       const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief カメラフレーム復帰動作を実行する
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  BoundingBoxDetectionResult result;                          // 検出結果
  int lineDirectionAngle;                                     // ラインの方向角度（絶対角度）
  int basePower;                                              // 基準パワー値
  PidGain anglePidGain;                                       // 角度制御用PIDゲイン
  int swingAngle;                                             // 首振り角度
  static constexpr int FRAME_NUMBER = 5;                      // フレーム取得回数
};

#endif