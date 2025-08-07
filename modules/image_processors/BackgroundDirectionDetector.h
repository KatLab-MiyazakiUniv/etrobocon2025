/**
 * @file   BackgroundDirectionDetector.h
 * @brief  風景の向きを判定する画像処理クラス
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

#define MODEL_INPUT_SIZE 640
#define CONFIDENCE_THRESHOLD 0.5f  // 検出結果を採用する最低信頼度の閾値
#define NMS_THRESHOLD 0.5f         // 検出ボックス同士の重なりを判断する閾値

// 風景の向きを表す列挙体
enum class BackgroundDirection { FRONT, RIGHT, BACK, LEFT };

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
  BackgroundDirectionDetector(const std::string& _modelPath
                              = "etrobocon2025/datafiles/models/BackgroundDetector.onnx");

  /**
   * @brief         入力画像から風景の向きを判定し、結果を result に格納する
   * @param frame   入力画像（cv::Mat）
   * @param result  検出結果（構造体）
   */
  void detect(const cv::Mat& frame, BackgroundDirectionResult& result);

 private:
  cv::dnn::Net net;             // DNNモデルを格納する変数
  const std::string modelPath;  // モデルのパス
  const std::string outputImagePath
      = "etrobocon2025/datafiles/processed_images/"
        "background_detected_result.jpg";  // バウンディングボックス付きの画像パス

  /**
   * @brief         入力画像を640x640にリサイズ＆パディングする前処理関数
   * @param frame   入力画像
   * @param scale   スケール係数
   * @param padX    X方向のパディング量
   * @param padY    Y方向のパディング量
   * @return        パディング後の画像
   */
  cv::Mat preprocess(const cv::Mat& frame, float scale, int padX, int padY);

  /**
   * @brief 出力結果を後処理して検出結果を生成する関数
   * @param outputs ネットワークの出力結果
   * @param frame   入力画像フレーム
   * @param scale   スケール係数
   * @param padX    X方向のパディング量
   * @param padY    Y方向のパディング量
   * @param result  検出結果を格納する構造体
   */
  void postprocess(const std::vector<cv::Mat>& outputs, const cv::Mat& frame, float scale, int padX,
                   int padY, BackgroundDirectionResult& result);
};

#endif  // BACKGROUND_DIRECTION_DETECTOR_H