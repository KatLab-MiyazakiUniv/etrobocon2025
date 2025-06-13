/**
 * @file   LineDetector.h
 * @brief  ライントレース用の画像処理クラス
 * @author takuchi17
 */

#ifndef LiNE_TRACE_IMAGE_PROCESSOR_H
#define LiNE_TRACE_IMAGE_PROCESSOR_H

#include "ObjectDetector.h"

class LineDetector : public ObjectDetector {
 public:
  /**
   * コンストラクタ
   * @brief ライントレース用の画像処理クラスを初期化する
   * @param lowerHSV ライントレース対象の色の下限HSV値
   * @param upperHSV ライントレース対象の色の上限HSV値
   */
  LineDetector(const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV);
  /**
   * @brief 画像処理を実行する純粋仮想関数
   * @param frame 処理対象の画像フレーム
   * @return 検出結果を含むDetectionResult構造体
   */
  DetectionResult process(const cv::Mat& frame) override;

 private:
  cv::Scalar lowerHSV;  // ライントレース対象の色の下限HSV値
  cv::Scalar upperHSV;  // ライントレース対象の色の上限HSV値
};

#endif
