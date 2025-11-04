/**
 * @file DoubleLineBoundingBoxDetector.h
 * @brief 2色指定ライントレース用の画像処理クラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#ifndef DOUBLE_LINE_BOUNDING_BOX_DETECTOR_H
#define DOUBLE_LINE_BOUNDING_BOX_DETECTOR_H

#include "BoundingBoxDetector.h"
#include "SystemInfo.h"

class DoubleLineBoundingBoxDetector {
 public:
  // ROIと解像度を指定するオーバーロードコンストラクタ
  DoubleLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1, const cv::Scalar& _upperHSV1,
                                const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2,
                                const cv::Rect& _roi, const cv::Size& _resolution);

  // ROIを指定するオーバーロードコンストラクタ (解像度はデフォルト 640x480)
  DoubleLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1, const cv::Scalar& _upperHSV1,
                                const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2,
                                const cv::Rect& _roi);

  // デフォルトのROIおよび解像度を使用するコンストラクタ (ROIは 50, 240, 540, 240)
  DoubleLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1, const cv::Scalar& _upperHSV1,
                                const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2);

  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result);
  void saveDebugImages(const cv::Mat& originalFrame, const cv::Mat& mask1, const cv::Mat& mask2, 
                       const cv::Mat& combinedMask, const cv::Rect& roiRect, 
                       const BoundingBoxDetectionResult& result, const std::string& prefix = "debug");

 private:
  // 対象の輪郭とみなす最小面積（この閾値は調整）
  static constexpr double MIN_LINE_CONTOUR_AREA = 50.0;
  cv::Scalar lowerHSV1;
  cv::Scalar upperHSV1;
  cv::Scalar lowerHSV2;  // 2色目
  cv::Scalar upperHSV2;  // 2色目

  cv::Rect roi;
  cv::Size resolution;

  void validateParameters();
};