/**
 * @file   ObjectDetector.h
 * @brief  画像処理の親クラス
 * @author takuchi17
 */

#ifndef OBJECT_DETECTOR_H
#define OBJECT_DETECTOR_H

#include <string>
#include <opencv2/opencv.hpp>

struct DetectionResult {
  bool wasDetected = false;  // 検出できたかどうか
  std::string label;         // 検出したラベル
  cv::Point topLeft;         // 検出した領域の左上の座標
  cv::Point topRight;        // 検出した領域の右上の座標
  cv::Point bottomLeft;      // 検出した領域の左下の座標
  cv::Point bottomRight;     // 検出した領域の右下の座標
};

class ObjectDetector {
 public:
  /**
   * 仮想デストラクタ
   * @brief 派生クラスのデストラクタが正しく呼ばれるようにするために必要
   */
  virtual ~ObjectDetector() = default;

  /**
   * @brief 画像処理を実行する純粋仮想関数
   * @param frame 処理対象の画像フレーム
   * @return 検出結果を含むDetectionResult構造体
   */
  virtual DetectionResult process(const cv::Mat& frame) = 0;
};

#endif
