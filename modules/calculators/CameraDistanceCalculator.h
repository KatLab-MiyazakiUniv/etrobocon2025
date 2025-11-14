/**
 * @file   CameraDistanceCalculator.h
 * @brief  カメラ画像を使って、走行体から対象物までの距離を計算するクラス
 * @author nishijima515
 */

#ifndef CAMERA_DISTANCE_CALCULATOR_H
#define CAMERA_DISTANCE_CALCULATOR_H
#define BOTTOM_Y 600.0  // 画像の下端Y座標
#define BOTTOM_TO_ROBOT \
  135.0  // 画角の一番下から走行体までの実際の距離[mm] 紫破線から走行体までの距離 130から150
#define BLUECIRCLE_REAL_WIDTH 40.0  // 青丸の実際の幅[mm]

#include "SystemInfo.h"
#include "SocketProtocol.h"
#include "Robot.h"

class CameraDistanceCalculator {
 public:
  /**
   * @brief コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _offsetDistance 距離補正値[mm]
   */
  CameraDistanceCalculator(Robot& _robot, double _offsetDistance = 0.0);

  /**
   * @brief カメラ画像を使って、走行体からバウンディングボックスの中心までの距離を計算する
   * @param response カメラサーバーからの応答
   * @param offsetDistance 距離補正値[mm]
   * @return バウンディングボックスの中心までの距離[mm]
   */
  double calculateDistance(const CameraServer::BoundingBoxDetectorResponse& response,
                           double offsetDistance);

 private:
  Robot& robot;
  double offsetDistance = 0.0;  // 距離補正値[mm]
};

#endif
