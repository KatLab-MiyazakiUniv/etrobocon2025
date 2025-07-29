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
#include <array>

class CameraRecoveryAction : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _boundingBoxDetector 画像処理クラスの参照
   * @param _targetXCoordinate 目標X座標（復帰判定用）
   */
  CameraRecoveryAction(Robot& _robot, BoundingBoxDetector& _boundingBoxDetector,
                       int _targetXCoordinate);

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
  BoundingBoxDetector& boundingBoxDetector;  // 画像処理クラスの参照
  BoundingBoxDetectionResult result;         // 検出結果
  int targetXCoordinate = 400;               // 目標X座標
  bool recoverySuccess = false;              // 復帰成功フラグ

  static constexpr std::array<int, 3> RECOVERY_ANGLES = { 5, 10, 15 };  // 復帰回頭角度設定
  static constexpr int RECOVERY_SPEED = 50;                             // 回頭スピード
  static constexpr double MAX_POSITION_DEVIATION = 100.0;               // 最大位置偏差

  /**
   * @brief 左右対称の段階的回頭による復帰処理
   * @return true: 検出成功, false: 検出失敗
   */
  bool rotationRecovery();

  /**
   * @brief 指定した方向・角度に回頭して線の検出を試行
   * @param angle 角度
   * @param isClockwise 回頭方向（true: 右回り, false: 左回り）
   * @return true: 検出成功, false: 検出失敗
   */
  bool tryRotationDirection(int angle, bool isClockwise);

  /**
   * @brief 検出された位置が妥当かチェック
   * @param detectedX 検出されたX座標
   * @return true: 正常, false: 異常
   */
  bool isDetectedPositionValid(double detectedX) const;
};

#endif