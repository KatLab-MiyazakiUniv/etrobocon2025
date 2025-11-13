/**
 * @file   OrthogonalLineRecoveryAction.h
 * @brief  ライン方向の法線に沿って前進して復帰を試みる動作
 * @author Hara1274
 */

#ifndef ORTHOGONAL_LINE_RECOVERY_ACTION_H
#define ORTHOGONAL_LINE_RECOVERY_ACTION_H

#include "CompositeMotion.h"
#include "IMUAngleRotation.h"
#include "SocketProtocol.h"
#include "Snapshot.h"
#include "Pid.h"
#include "PictureColorDistanceStraight.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

class OrthogonalLineRecoveryAction : public CompositeMotion {
 public:
  /**
   * @brief コンストラクタ
   * @param _robot ロボット本体
   * @param _lineDirectionAngle ライン方向の絶対角度（deg）
   * @param _basePower 回頭時の基準パワー値
   * @param _anglePidGain 角度制御用PIDゲイン
   * @param _orthogonalDistance 法線方向へ前進する距離[mm]
   * @param _orthogonalSpeed 法線方向の前進速度[mm/s]
   * @param _pcidsPidGain 法線直進用PIDゲイン
   * @param _lineDetectionRequest ライン検出リクエスト
   * @param _pcidsDetectionRequest PCIDS用の検出リクエスト
   */
  OrthogonalLineRecoveryAction(
      Robot& _robot, int _lineDirectionAngle, int _basePower, const PidGain& _anglePidGain,
      double _orthogonalDistance, double _orthogonalSpeed, const PidGain& _pcidsPidGain,
      const CameraServer::BoundingBoxDetectorRequest& _lineDetectionRequest,
      const CameraServer::BoundingBoxDetectorRequest& _pcidsDetectionRequest);

  /**
   * @brief 法線方向アプローチでライン復帰を試みる
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest lineDetectionRequest;   // ライン検出リクエスト
  CameraServer::BoundingBoxDetectorRequest pcidsDetectionRequest;  // PCIDS用検出リクエスト
  int lineDirectionAngle;                                          // ライン方向絶対角度
  int basePower;                                                   // 回頭用基準パワー
  PidGain anglePidGain;                                            // 角度制御PIDゲイン
  double orthogonalDistance;                                       // 法線方向の前進距離[mm]
  double orthogonalSpeed;                                          // 法線方向の前進速度[mm/s]
  PidGain pcidsPidGain;                                            // 法線直進用PIDゲイン
};

#endif
