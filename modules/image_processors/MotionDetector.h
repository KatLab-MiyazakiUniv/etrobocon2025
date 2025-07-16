/**
 * @file   MotionDetector.h
 * @brief  動体検知用の画像処理クラス
 * @author HaruArima08
 */

#ifndef MOTION_DETECTOR_H
#define MOTION_DETECTOR_H

#include "BoundingBoxDetector.h"
#include <opencv2/opencv.hpp>

class MotionDetector : public BoundingBoxDetector {
 public:
  /**
   * コンストラクタ
   * @param _threshold 差分の閾値（この値より大きい差分を動体として検出する）
   * @param _minArea 動体として認識する最小の輪郭面積
   * @param roi 注目領域
   */
  MotionDetector(double _threshold, double _minArea, const cv::Rect& _roi);

  /**
   * @brief         静的な背景画像を設定する
   * @param background グレースケール化された静的な背景画像
   */
  void setBackground(const cv::Mat& background);

  /**
   * @brief         入力画像から動体を検知し、そのバウンディングボックスを返す
   * @param frame   入力画像（cv::Mat）
   * @param result  検出結果を格納する構造体
   */
  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) override;

 private:
  double threshold;       // 差分の閾値
  double minArea;  // 動体として認識する最小の輪郭面積
  cv::Rect roi;         // 注目領域
  cv::Mat bgModel;        // 背景モデル
};

#endif
