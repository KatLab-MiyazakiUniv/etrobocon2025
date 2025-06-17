/**
 * @file   BackgroundBoundingBoxDetector.h
 * @brief  風景向き判定システム用の画像処理クラス
 * @author Hara1274
 */

#ifndef BACKGROUND_BOUNDING_BOX_DETECTOR_H
#define BACKGROUND_BOUNDING_BOX_DETECTOR_H

#include "BoundingBoxDetector.h"

class BackgroundBoundingBoxDetector : public BoundingBoxDetector {
 public:
  /**
   * @brief 風景向き判定システム用の画像処理クラスを初期化する
   */
  BackgroundBoundingBoxDetector();
  /**
   * @brief 画像処理を実行する純粋仮想関数
   * @param frame 処理対象の画像フレーム
   * @param result 結果を格納するBoundingBoxDetectionResult構造体の参照
   */
  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) override;
};

#endif