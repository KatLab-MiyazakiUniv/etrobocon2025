/**
 * @file   LineBoundingBoxDetector.h
 * @brief  ライントレース用の画像処理クラス
 * @author takuchi17
 */

#ifndef LiNE_BOUNDING_BOX_DETECTOR_H
#define LiNE_BOUNDING_BOX_DETECTOR_H

#include "BoundingBoxDetector.h"

class LineBoundingBoxDetector : public BoundingBoxDetector {
 public:
  /**
   * コンストラクタ
   * @brief ライントレース用の画像処理クラスを初期化する
   * @param lowerHSV ライントレース対象の色の下限HSV値
   * @param upperHSV ライントレース対象の色の上限HSV値
   */
  LineBoundingBoxDetector(const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV);
  /**
   * @brief 画像処理を実行する純粋仮想関数
   * @param frame 処理対象の画像フレーム
   * @param rusult バウンディングボックスを格納する構造体
   */
  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) override;

 private:
  cv::Scalar lowerHSV;  // ライントレース対象の色の下限HSV値
  cv::Scalar upperHSV;  // ライントレース対象の色の上限HSV値
};

#endif
