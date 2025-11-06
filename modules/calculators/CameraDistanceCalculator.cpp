// /**
//  * @file   CameraDistanceCalculator.cpp
//  * @brief  カメラ画像を使って、走行体から対象物までの距離を計算するクラス
//  * @author nishijima515
//  */

// #include "CameraDistanceCalculator.h"

// CameraDistanceCalculator::CameraDistanceCalculator(Robot& _robot) : robot(_robot) {}

// double CameraDistanceCalculator::calculateDistance(
//     const CameraServer::BoundingBoxDetectorResponse& response)
// {
//   // バウンディングボックスの中心座標を取得
//   double currentX = (response.result.topLeft.x + response.result.bottomRight.x) / 2.0;
//   double currentY = (response.result.topLeft.y + response.result.bottomRight.y) / 2.0;

//   // 対象物から画角の一番下までのピクセル数を計算
//   double bottomToCurrentY = BOTTOM_Y - currentY;

//   // コース上での実際の長さに変換(スマートキャリーのランディング用)

//   double onePixelDistance
//       = BLUECIRCLE_REAL_WIDTH / (response.result.bottomRight.x - response.result.topLeft.x);
//   double distance = (bottomToCurrentY * onePixelDistance) + BOTTOM_TO_ROBOT;

//   return distance;
// }

#include "CameraDistanceCalculator.h"
#include <cmath>
#include <limits>

namespace {
  double distanceFromPixelToGround_mm(double u, double v, double fx, double fy, double cx,
                                      double cy, double cameraHeightMm, double tiltDeg)
  {
    double x_d = (u - cx) / fx;
    double y_d = (v - cy) / fy;
    double theta = tiltDeg * M_PI / 180.0;

    double denom = std::sin(theta) * y_d + std::cos(theta);
    if(std::abs(denom) < 1e-6) return std::numeric_limits<double>::quiet_NaN();

    double s = -cameraHeightMm / denom;

    double Xw = s * x_d;
    double Yw = s * (std::cos(theta) * y_d - std::sin(theta));

    return std::sqrt(Xw * Xw + Yw * Yw);
  }
}  // namespace

CameraDistanceCalculator::CameraDistanceCalculator(Robot& _robot) : robot(_robot) {}

double CameraDistanceCalculator::calculateDistance(
    const CameraServer::BoundingBoxDetectorResponse& response)
{
  const double IMG_W = 800.0;
  const double IMG_H = 600.0;
  const double cx = IMG_W / 2.0;
  const double cy = IMG_H / 2.0;

  // ---- あなたのカメラに合わせたパラメータ ----
  const double fx = 500.0;  // 3.6mmレンズ → 約78°画角 → ≈500px
  const double fy = 520.0;
  const double cameraHeight = 210.0;  // mm
  const double tiltDeg = 70.0;

  double u = (response.result.topLeft.x + response.result.bottomRight.x) / 2.0;
  double v = response.result.bottomRight.y;

  double distMm = distanceFromPixelToGround_mm(u, v, fx, fy, cx, cy, cameraHeight, tiltDeg);
  return distMm;  // cm単位で返す
}