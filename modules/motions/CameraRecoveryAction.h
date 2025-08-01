/**
 * @file   CameraRecoveryAction.h
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#ifndef CAMERA_RECOVERY_ACTION_H
#define CAMERA_RECOVERY_ACTION_H

#include "Motion.h"
#include "BoundingBoxDetector.h"
#include "AngleRotation.h"

class CameraRecoveryAction : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _angle 回頭角度 (dig) 0~360
   * @param _speed 回頭速度（mm/秒）
   * @param _isClockwise 回頭方向（true: 右回り, false: 左回り）
   * @param _boundingBoxDetector 画像処理クラスのポインタ
   */
  CameraRecoveryAction(Robot& _robot, int _angle, double _speed, bool _isClockwise,
                       std::unique_ptr<BoundingBoxDetector> _boundingBoxDetector);

  /**
   * @brief カメラフレーム復帰動作を実行する
   */
  void run() override;

  /**
   * @brief 復帰動作が成功したかチェック
   * @return true: 復帰成功, false: 復帰失敗
   */
  bool isRecoverySuccessful() const;

 private:
  std::unique_ptr<BoundingBoxDetector> boundingBoxDetector;  // 画像処理クラスのポインタ
  BoundingBoxDetectionResult result;                         // 検出結果
  int recoveryAngle;                                         // 復帰回頭角度
  double speed;                                              // 回頭スピード
  bool isClockwise;                                          // 回頭方向
  static constexpr int FRAME_NUMBER = 5;                     // フレーム取得回数
  bool recoverySuccess = false;                              // 復帰成功フラグ
};

#endif