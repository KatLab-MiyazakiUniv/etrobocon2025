/**
 * @file   BoundingBoxDetector.h
 * @brief  画像処理の親クラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#ifndef BOUNDING_BOX_DETECTOR_H
#define BOUNDING_BOX_DETECTOR_H

#include <string>
#include <opencv2/opencv.hpp>

struct BoundingBoxDetectionResult {
  bool wasDetected = false;  // 検出できたかどうか
  cv::Point topLeft;         // 検出した領域の左上の座標
  cv::Point topRight;        // 検出した領域の右上の座標
  cv::Point bottomLeft;      // 検出した領域の左下の座標
  cv::Point bottomRight;     // 検出した領域の右下の座標
};

class BoundingBoxDetector {
 public:
  /**
   * 仮想デストラクタ
   * @brief 派生クラスのデストラクタが正しく呼ばれるようにするために必要
   */
  virtual ~BoundingBoxDetector() = default;

  /**
   * @brief 物体検出を実行する純粋仮想関数
   * @param frame 処理対象の画像フレーム
   * @param result 結果を格納するBoundingBoxDetectionResult構造体の参照
   */
  virtual void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) = 0;
};

#endif
