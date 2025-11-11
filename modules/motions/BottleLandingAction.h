/**
 * @file   BottleLandingAction.h
 * @brief  ボトルランディング動作クラス
 * @author nishijima515
 */

#ifndef BOTTLE_LANDING_ACTION_H
#define BOTTLE_LANDING_ACTION_H
#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "IMUDistanceStraight.h"
#include "CameraDistanceCalculator.h"
#include "SocketProtocol.h"
#include "ColorJudge.h"
#include <opencv2/opencv.hpp>  // For cv::Rect

class BottleLandingAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _offsetDistance 距離補正値[mm]
   * @param _idsSpeed IDSの走行速度[mm/s]
   * @param _detectionRequest 検出リクエスト
   */
  BottleLandingAction(Robot& _robot, double _offsetDistance, double _idsSpeed,
                      const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief ボトルランディング動作を行う
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  BoundingBoxDetectionResult result;  // バウンディングボックスの座標を格納する構造体
  double offsetDistance;              // 距離補正値[mm]
  double idsSpeed;                    // IDSの走行速度[mm/s]
};

#endif
