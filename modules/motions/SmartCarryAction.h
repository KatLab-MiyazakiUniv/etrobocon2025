/**
 * @file   SmartCarryAction.h
 * @brief  スマートキャリー動作クラス(ボトル2のランディングのみ)
 * @author nishijima515
 */

#ifndef SMART_CARRY_ACTION_H
#define SMART_CARRY_ACTION_H
#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "IMURotation.h"
#include "DistanceStraight.h"
#include "UltrasonicDistanceCameraLineTrace.h"
#include "LineBoundingBoxDetector.h"

class SmartCarryAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   */
  SmartCarryAction(Robot& _robot, double _ultrasonicDistance, int _imuDeg, double _imuPower,
                   bool _isClockwise, double _dsSpeed);

  /**
   * @brief スマートキャリー動作を行う
   */
  void run() override;

 private:
  double ultrasonicDistance;
  int imuDeg;
  double imuPower;
  bool isClockwise;
  double dsSpeed;
  cv::Scalar lowerHSV = cv::Scalar(85, 50, 50);
  cv::Scalar upperHSV = cv::Scalar(105, 255, 255);
  cv::Rect roi = cv::Rect(0, 0, 600, 600);
  cv::Size resolution = cv::Size(600, 600);
  std::unique_ptr<BoundingBoxDetector> detector
      = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV, roi, resolution);
};

#endif
