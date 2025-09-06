/**
 * @file   MotionDetector.h
 * @brief  動体検知用の画像処理クラス
 * @author HaruArima08 miyahara046
 */

#ifndef MOTION_DETECTOR_H
#define MOTION_DETECTOR_H

#include "BoundingBoxDetector.h"
#include "FrameSave.h"
#include <opencv2/opencv.hpp>

class MotionDetector : public BoundingBoxDetector {
 public:
  /**
   * @brief コンストラクタ
   * @param _threshold 差分画像の二値化に用いる閾値
   * @param _minArea 検出対象とする輪郭の最小面積
   * @param _roi 動体検知を行う範囲（Region of Interest）
   */
  MotionDetector(double _threshold, double _minArea, const cv::Rect& _roi);

  /**
   * @brief 背景画像の設定
   * @param background 背景として使用する画像
   * @return true 成功した場合
   * @return false 失敗した場合
   */
  bool setBackground(const cv::Mat& background);

  /**
   * @brief 現在のフレームと背景画像を比較して動体を検出
   * @param frame 現在のフレーム
   * @param result 検出結果(バウンディングボックス)を格納する構造体
   */
  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) override;

  /**
   * @brief 2枚のフレームを比較して差分の最大輪郭を取得する
   * @param firstFrame 最初のフレーム
   * @param secondFrame 比較対象のフレーム
   * @return 差分の最大輪郭
   */
  std::vector<cv::Point> compareTwoFrames(const cv::Mat& firstFrame, const cv::Mat& secondFrame);

 private:
  double threshold;  // 差分画像の二値化に用いる閾値
  double minArea;    // 検出対象とする輪郭の最小面積
  cv::Rect roi;      // 動体検知を行う範囲（Region of Interest）
  cv::Mat bgFrame;   // 背景画像
};

#endif
