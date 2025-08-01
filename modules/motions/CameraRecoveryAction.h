/**
 * @file   CameraRecoveryAction.h
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#ifndef CAMERA_RECOVERY_ACTION_H
#define CAMERA_RECOVERY_ACTION_H

#include "Motion.h"
#include "BoundingBoxDetector.h"
#include "LineBoundingBoxDetector.h"
#include "AngleRotation.h"

class CameraRecoveryAction : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _angle 回頭角度
   * @param _isClockwise 回頭方向（true: 右回り, false: 左回り）
   * @param _lowerHSV ライントレース対象の色の下限HSV値
   * @param _upperHSV ライントレース対象の色の上限HSV値
   */
  CameraRecoveryAction(Robot& _robot, int _angle, bool _isClockwise, cv::Scalar _lowerHSV,
                       cv::Scalar _upperHSV);

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
  std::unique_ptr<BoundingBoxDetector> boundingBoxDetector;  // 画像処理クラス
  BoundingBoxDetectionResult result;                         // 検出結果
  int recoveryAngle;                                         // 復帰回頭角度
  bool isClockwise;                                          // 回頭方向
  bool recoverySuccess = false;                              // 復帰成功フラグ
  static constexpr int RECOVERY_SPEED = 100;                 // 回頭スピード
};

#endif