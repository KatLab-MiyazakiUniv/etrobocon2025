/**
 * @file   MiniFigDirectionDetector.h
 * @brief  ミニフィグの向きを検出するクラス
 * @author nishijima515
 */

#ifndef MINIFIG_DIRECTION_DETECTOR_H
#define MINIFIG_DIRECTION_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#define MODEL_INPUT_SIZE 640       // モデルの入力サイズ（640x640）
#define CONFIDENCE_THRESHOLD 0.5f  // 検出結果を採用する最低信頼度の閾値
#define NMS_THRESHOLD 0.5f         // 検出ボックス同士の重なりを判断する閾値

// ミニフィグの向きを表す列挙体
enum class MiniFigDirection { FRONT, RIGHT, BACK, LEFT };

struct MiniFigDirectionResult {
  bool wasDetected = false;    // 検出が成功したかどうか
  MiniFigDirection direction;  // ミニフィグの向きを表す列挙体
};

class MiniFigDirectionDetector {
 public:
  // コンストラクタ
  MiniFigDirectionDetector();

  /**
   * @brief         入力画像からミニフィグの向きを判定し、結果を result に格納する
   * @param frame   入力画像（cv::Mat）
   * @param result  検出結果（構造体）
   */
  void detect(const cv::Mat& frame, MiniFigDirectionResult& result);

  /**
   * @brief             モデルの読み込みを行う
   * @param modelPath   モデルパス
   */
  static void initializeModel(const std::string& modelPath
                              = "etrobocon2025/datafiles/models/yolo_optimized.onnx");

 private:
  static cv::dnn::Net net;  // DNNモデルを格納する変数
  static bool isInitialized;  // モデルの読み込みを行ったかどうかの状態を表す静的変数
  const std::string outputImagePath
      = "etrobocon2025/datafiles/processed_images/"
        "fig_detected_result.jpg";  // バウンディングボックス付きの画像のパス

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
                   int padY, MiniFigDirectionResult& result);
};

#endif  // MINIFIG_DIRECTION_DETECTOR_H