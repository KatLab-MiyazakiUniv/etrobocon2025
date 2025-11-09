/**
 * @file   CameraDistanceCalculator.cpp
 * @brief  カメラ画像を使って、走行体から対象物までの距離を計算するクラス
 * @author nishijima515
 */

#include "CameraDistanceCalculator.h"

CameraDistanceCalculator::CameraDistanceCalculator(Robot& _robot, double _offsetDistance)
  : robot(_robot), offsetDistance(_offsetDistance)
{
}

double CameraDistanceCalculator::calculateDistance(
    const CameraServer::BoundingBoxDetectorResponse& response, double offsetDistance)
{
  // バウンディングボックスの中心座標を取得
  double currentX = (response.result.topLeft.x + response.result.bottomRight.x) / 2.0;
  double currentY = (response.result.topLeft.y + response.result.bottomRight.y) / 2.0;

  // 対象物から画角の一番下までのピクセル数を計算
  double bottomToCurrentY = BOTTOM_Y - currentY;

  // コース上での実際の長さに変換(スマートキャリーのランディング用)

  double onePixelDistance
      = BLUECIRCLE_REAL_WIDTH / (response.result.bottomRight.x - response.result.topLeft.x);
  double distance = (bottomToCurrentY * onePixelDistance) + BOTTOM_TO_ROBOT + offsetDistance;

  return distance;
}