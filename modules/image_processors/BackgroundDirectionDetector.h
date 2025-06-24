/**
 * @file   BackgroundDirectionDetector.h
 * @brief  風景の向きを判定のするクラス
 * @author Hara1274
 */

#ifndef BACKGROUND_DIRECTION_DETECTOR_H
#define BACKGROUND_DIRECTION_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#define CONFIDENCE_THRESHOLD 0.5  // 検出結果を採用する最低信頼度の閾値
#define NMS_THRESHOLD 0.5         // 検出ボックス同士の重なりを判断する閾値

// 風景の向きを表す列挙体
// enum class BackgroundDirection { FRONT, LEFT, BACK, RIGHT };
enum class BackgroundDirection { FRONT, BACK, RIGHT, LEFT };

// 結果格納用の構造体
struct BackgroundDirectionResult {
  bool wasDetected = false;       // 検出が成功したかどうか
  BackgroundDirection direction;  // 風景の向きを表す列挙体
};

class BackgroundDirectionDetector {
 public:
  /**
   * コンストラクタ
   */
  BackgroundDirectionDetector(const std::string& modelPath);

  /**
   * @brief         入力画像から風景の向きを判定し、結果を result に格納する
   * @param frame   入力画像（cv::Mat）
   * @param result  検出結果（構造体）
   */
  void detect(const cv::Mat& frame, BackgroundDirectionResult& result);

 private:
  cv::dnn::Net net;  // DNNモデルを格納する変数

  /**
   * @brief         入力画像を640x640にリサイズ＆パディングする前処理関数
   * @param frame   入力画像
   * @return        パディング後の画像
   */
  cv::Mat preprocess(const cv::Mat& frame);
};

#endif  // BACKGROUND_DIRECTION_DETECTOR_H