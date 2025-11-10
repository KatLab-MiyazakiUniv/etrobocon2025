/**
 * @file   BottleTwoCatchAction.h
 * @brief  ボトル2つ目のキャッチ動作クラス
 * @author nishijima515
 */

#ifndef BOTTLE_TWO_CATCH_ACTION_H
#define BOTTLE_TWO_CATCH_ACTION_H
#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "IMUDistanceStraight.h"
#include "IMUAngleRotation.h"
#include "UltrasonicDistanceCameraLineTrace.h"
#include "SocketProtocol.h"
#include "PictureColorDistanceStraight.h"
#include "IMUMinAngleRotation.h"
#include <cmath>

class BottleTwoCatchAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   */
  BottleTwoCatchAction(Robot& _robot, double _forwardDistance, double _idsSpeed,
                       double _ultrasonicDistance, double _udclSpeed, double _angle,
                       double _rotatePower,
                       const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief スマートキャリー動作を行う
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  BoundingBoxDetectionResult result;  // バウンディングボックスの座標を格納する構造体
  double forwardDistance;
  double idsSpeed;
  double ultrasonicDistance;
  double udclSpeed;
  double angle;
  double rotatePower;
  //   cv::Scalar lowerHSV = cv::Scalar(85, 50, 50);
  //   cv::Scalar upperHSV = cv::Scalar(105, 255, 255);
  //   cv::Rect roi = cv::Rect(0, 0, 600, 600);
  //   cv::Size resolution = cv::Size(600, 600);
  //   std::unique_ptr<BoundingBoxDetector> detector
  //       = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV, roi, resolution);
};

#endif

// SmartCarryAction::SmartCarryAction(
//     Robot& _robot, double _forwardDistance, double _idsSpeed, double _ultrasonicDistance,
//     double _udclpeed, double _angle, double _rotatePower,
//     const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
