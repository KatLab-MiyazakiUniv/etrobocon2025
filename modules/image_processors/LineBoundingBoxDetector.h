/**
 * @file   LineBoundingBoxDetector.h
 * @brief  ライントレース用の画像処理クラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#ifndef LINE_BOUNDING_BOX_DETECTOR_H
#define LINE_BOUNDING_BOX_DETECTOR_H

#include "BoundingBoxDetector.h"

// 解像度の最小・最大値
struct ResolutionRange {
  static constexpr int MIN_WIDTH = 320;
  static constexpr int MIN_HEIGHT = 240;
  static constexpr int MAX_WIDTH = 1920;
  static constexpr int MAX_HEIGHT = 1080;
};

class LineBoundingBoxDetector : public BoundingBoxDetector {
 public:
  /**
   * @brief デフォルトのROIおよび解像度を使用するコンストラクタ
   * @param lowerHSV ライントレース対象の色の下限HSV値
   * @param upperHSV ライントレース対象の色の上限HSV値
   */
  LineBoundingBoxDetector(const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV);

  /**
   * @brief ROIを指定するオーバーロードコンストラクタ
   * @param lowerHSV ライントレース対象の色の下限HSV値
   * @param upperHSV ライントレース対象の色の上限HSV値
   * @param roi 注目領域
   */
  LineBoundingBoxDetector(const cv::Scalar& lowerHSV, const cv::Scalar& upperHSV,
                          const cv::Rect& roi);

  /**
   * @brief ROIと解像度を指定するオーバーロードコンストラクタ
   * @param lowerHSV ライントレース対象の色の下限HSV値
   * @param upperHSV ライントレース対象の色の上限HSV値
   * @param roi 注目領域
   * @param resolution 画像処理に用いる解像度
   */
  LineBoundingBoxDetector(const cv::Scalar& lowerHSV, const cv::Scalar& upperHSV,
                          const cv::Rect& roi, const cv::Size& resolution);

  /**
   * @brief 画像処理を実行する関数
   * @param frame 処理対象の画像フレーム
   * @param result 結果を格納するBoundingBoxDetectionResult構造体の参照
   */
  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) override;

 private:
  // ラインの輪郭とみなす最小面積（この閾値は調整）
  static constexpr double MIN_LINE_CONTOUR_AREA = 50.0;
  cv::Scalar lowerHSV;  // ライントレース対象の色の下限HSV値
  cv::Scalar upperHSV;  // ライントレース対象の色の上限HSV値
  cv::Rect roi;         // 注目領域
  cv::Size resolution;  // 解像度

  /**
   * @brief 入力で受け取ったROIと解像度の検証を行う関数
   */
  void validateParameters();
};

#endif
