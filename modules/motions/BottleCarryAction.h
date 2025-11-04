/**
 * @file   BottleCarryAction.h
 * @brief  ボトルキャリー動作クラス
 * @author nishijima515
 */

#ifndef BOTTLE_CARRY_ACTION_H
#define BOTTLE_CARRY_ACTION_H
#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "IMUDistanceStraight.h"
#include "IMUColorDistanceStraight.h"
#include "IMUAngleRotation.h"
#include "GetCorrectionAngle.h"
#include "IMUSetting.h"
#include "CameraDistanceCalculator.h"
#include "SocketProtocol.h"
#include "ColorJudge.h"

class BottleCarryAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   */
  BottleCarryAction(Robot& _robot, double _forwardDistance, double _maxDistance, double _idsSpeed,
                    int _targetXCoordinate,
                    const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

  /**
   * @brief ボトルキャリー動作を行う
   */
  void run() override;

 private:
  CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
  BoundingBoxDetectionResult result;  // バウンディングボックスの座標を格納する構造体
  double forwardDistance;
  double idsSpeed;
  double maxDistance;
  int targetXCoordinate;
};

#endif
