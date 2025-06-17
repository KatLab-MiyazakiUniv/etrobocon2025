/**
 * @file   BackgroundBoundingBoxDetector.cpp
 * @brief  風景向き判定システム用の画像処理クラス
 * @author Hara1274
 */

#include "BackgroundBoundingBoxDetector.h"

BackgroundBoundingBoxDetector::BackgroundBoundingBoxDetector() {}

void BackgroundBoundingBoxDetector::detect(const cv::Mat& frame, BoundingBoxDetectionResult& result)
{
  // 注目領域の設定
  // 例: 解像度 640x480 の画像の場合
  const int roi_x = 50;        // 左端からの開始X座標
  const int roi_y = 30;        // 上端からの開始Y座標 (画像上側から開始)
  const int roi_width = 540;   // ROIの幅 (例: 640 - 50 - 50 = 540)
  const int roi_height = 240;  // ROIの高さ (例: 480 - 140 = 340, 340に設定)

  cv::Rect roiRect(roi_x, roi_y, roi_width, roi_height);

  // frameが空でないか、または画像範囲外でないかのチェック
  if(frame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return;
  }

  // ROIが画像サイズを超えないようにクリップする処理
  roiRect = roiRect & cv::Rect(0, 0, frame.cols, frame.rows);
  // クリップした結果ROIが空になった場合
  if(roiRect.empty()) {
    std::cerr << "Error: ROI is empty after clipping." << std::endl;
    return;
  }

  // 入力画像を複製し、ROI領域を赤枠で描画
  cv::Mat debugROIImage = frame.clone();
  cv::rectangle(debugROIImage, roiRect, cv::Scalar(0, 0, 255), 2);  // 赤色(BGR: 0,0,255)
  cv::imwrite("etrobocon2025/datafiles/snapshots/debug_roi.JPEG", debugROIImage);

  // ROIを切り出す
  cv::Mat roiFrame = frame(roiRect);

  // 白色のHSVの下限・上限の例（要調整）
  cv::Scalar lowerHSV(0, 0, 200);
  cv::Scalar upperHSV(180, 30, 255);

  // フレームをHSV色空間に変換
  cv::Mat hsvFrame;
  cv::cvtColor(roiFrame, hsvFrame, cv::COLOR_BGR2HSV);

  // HSV範囲でマスクを設定
  cv::Mat mask;
  cv::inRange(hsvFrame, lowerHSV, upperHSV, mask);

  // デバッグ表示
  cv::imwrite("etrobocon2025/datafiles/snapshots/debug1.JPEG", mask);

  // マスクの輪郭を検出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
}