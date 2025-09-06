/**
 * @file   CameraRecoveryAction.h
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#ifndef CAMERA_RECOVERY_ACTION_H
#define CAMERA_RECOVERY_ACTION_H

#include "CompositeMotion.h"
#include "AngleRotation.h"
#include "SocketProtocol.h"

class CameraRecoveryAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _angle 回頭角度 (dig) 0~180
   * @param _speed 回頭速度（mm/秒）
   * @param _isClockwise 回頭方向（true: 右回り, false: 左回り）
   * @param _detectionRequest 検出リクエスト
   */
  CameraRecoveryAction(Robot& _robot, int _angle, double _speed, bool _isClockwise,
                       const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief カメラフレーム復帰動作を実行する
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  BoundingBoxDetectionResult result;                          // 検出結果
  int recoveryAngle;                                          // 復帰回頭角度
  double speed;                                               // 回頭スピード
  bool isClockwise;                                           // 回頭方向
  static constexpr int FRAME_NUMBER = 5;                      // フレーム取得回数
};

#endif