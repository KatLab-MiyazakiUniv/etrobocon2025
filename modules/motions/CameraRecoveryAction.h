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
#include "PictureColorDistanceStraight.h"
#include "Pid.h"
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

class CameraRecoveryAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _lineDirectionAngle ラインの方向角度（絶対角度, deg） 0~360
   * @param _basePower 基準パワー値
   * @param _anglePidGain 角度制御用PIDゲイン
   * @param _swingAngle 首振り角度（deg）
   * @param _maxSwingCount 最大首振り回数
   * @param _pcidsDistance PCIDS用の目標距離[mm]
   * @param _pcidsSpeed PCIDS用の目標速度[mm/s]
   * @param _pcidsPidGain PCIDS用の角度補正PIDゲイン
   * @param _detectionRequest 検出リクエスト
   */
  CameraRecoveryAction(Robot& _robot, int _lineDirectionAngle, int _basePower,
                       const PidGain& _anglePidGain, int _swingAngle, int _maxSwingCount,
                       double _pcidsDistance, double _pcidsSpeed, const PidGain& _pcidsPidGain,
                       const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief カメラフレーム復帰動作を実行する
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  BoundingBoxDetectionResult result;                          // 検出結果
  int lineDirectionAngle;                 // ラインの方向角度（絶対角度）
  int basePower;                          // 基準パワー値
  PidGain anglePidGain;                   // 角度制御用PIDゲイン
  int swingAngle;                         // 首振り角度
  int maxSwingCount;                      // 最大首振り回数
  double pcidsDistance;                   // PCIDS用の目標距離[mm]
  double pcidsSpeed;                      // PCIDS用の目標速度[mm/s]
  PidGain pcidsPidGain;                   // PCIDS用の角度補正PIDゲイン
  static constexpr int FRAME_NUMBER = 5;  // フレーム取得回数
};

#endif