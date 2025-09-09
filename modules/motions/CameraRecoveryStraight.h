/**
 * @file   CameraRecoveryStraight.h
 * @brief  カメラ検出失敗時の復帰直線動作クラス
 * @author nishijima515
 */

#ifndef CAMERA_RECOVERY_STRAIGHT_H
#define CAMERA_RECOVERY_STRAIGHT_H

#include "CompositeMotion.h"
#include "BoundingBoxDetector.h"
#include "DistanceStraight.h"

class CameraRecoveryStraight : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _straightDistance 直進距離（mm）
   * @param _targetDistance 復帰目標距離（mm）
   * @param _speed 直線速度（mm/秒）
   * @param _boundingBoxDetector 画像処理クラスのポインタ
   */
  CameraRecoveryStraight(Robot& _robot, double _straightDistance, double _targetDistance, double _speed,
                       std::unique_ptr<BoundingBoxDetector> _boundingBoxDetector);

  /**
   * @brief カメラフレーム復帰動作を実行する
   */
  void run() override;

 private:
  std::unique_ptr<BoundingBoxDetector> boundingBoxDetector;  // 画像処理クラスのポインタ
  BoundingBoxDetectionResult result;                         // 検出結果
  double straightDistance;                                     // 復帰直進距離
  double targetDistance;                                       // 復帰目標距離
  double speed;                                              // 回頭スピード
  static constexpr int FRAME_NUMBER = 5;                     // フレーム取得回数
};

#endif