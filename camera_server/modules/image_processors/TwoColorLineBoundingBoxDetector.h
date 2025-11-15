/**
 * @file TwoColorLineBoundingBoxDetector.h
 * @brief 2色指定ライントレース用の画像処理クラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#ifndef TWO_COLOR_LINE_BOUNDING_BOX_DETECTOR_H
#define TWO_COLOR_LINE_BOUNDING_BOX_DETECTOR_H

#include "BoundingBoxDetector.h"
#include "SystemInfo.h"
#include "FrameSave.h"

class TwoColorLineBoundingBoxDetector {
 public:
  // ROIと解像度を指定するオーバーロードコンストラクタ
  TwoColorLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1, const cv::Scalar& _upperHSV1,
                                  const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2,
                                  const cv::Rect& _roi, const cv::Size& _resolution);

  // ROIを指定するオーバーロードコンストラクタ (解像度はデフォルト 640x480)
  TwoColorLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1, const cv::Scalar& _upperHSV1,
                                  const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2,
                                  const cv::Rect& _roi);

  // デフォルトのROIおよび解像度を使用するコンストラクタ (ROIは 50, 240, 540, 240)
  TwoColorLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1, const cv::Scalar& _upperHSV1,
                                  const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2);

  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result);

 private:
  // 対象の輪郭とみなす最小面積（この閾値は調整）
  static constexpr double MIN_LINE_CONTOUR_AREA = 50.0;
  cv::Scalar lowerHSV1;  // 1色目のライントレース対象の色の下限HSV値
  cv::Scalar upperHSV1;  // 1色目のライントレース対象の色の上限HSV値
  cv::Scalar lowerHSV2;  // 2色目のライントレース対象の色の下限HSV値
  cv::Scalar upperHSV2;  // 2色目ライントレース対象の色の上限HSV値
  cv::Rect roi;          // 注目領域
  cv::Size resolution;   // 画像処理に用いる解像度

  void validateParameters();
};
#endif  // TWO_COLOR_LINE_BOUNDING_BOX_DETECTOR_H