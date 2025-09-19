/**
 * @file   MiniFigDirectionDetector.h
 * @brief  ミニフィグの向きを検出するクラス
 * @author nishijima515
 */

#ifndef MINIFIG_DIRECTION_DETECTOR_H
#define MINIFIG_DIRECTION_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <onnxruntime_cxx_api.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "ImageRecognitionResults.h"
#include "FrameSave.h"

#define MODEL_INPUT_SIZE 672       // モデルの入力サイズ（640x640）
#define CONFIDENCE_THRESHOLD 0.5f  // 検出結果を採用する最低信頼度の閾値
#define NMS_THRESHOLD 0.5f         // 検出ボックス同士の重なりを判断する閾値

class MiniFigDirectionDetector {
 public:
  /**
   * @brief コンストラクタ
   * @param modelPath ONNXモデルのパス (デフォルト値あり)
   */
  MiniFigDirectionDetector(const std::string& modelPath = "datafiles/models/11s_125epoch_&_660imgsz_data0919_fig.onnx");

  /**
   * @brief         入力画像からミニフィグの向きを判定し、結果を result に格納する
   * @param frame   入力画像（cv::Mat）
   * @param result  検出結果（構造体）
   */
  void detect(const cv::Mat& frame, MiniFigDirectionResult& result);

 private:
  Ort::Env env;          // ONNX Runtime 環境
  Ort::Session session;  // 推論セッション
  std::vector<std::string> inputNames;
  std::vector<std::string> outputNames;
  const std::string modelPath;  // モデルのパス
  const std::string outputImagePath
      = "datafiles/processed_images/";  // バウンディングボックス付きの画像保存先ディレクトリ
  const std::string outputImageName
      = "minifig_detected_result";  // バウンディングボックス付きの画像保存名

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
  void postprocess(const std::vector<std::vector<float>>& outputs, const cv::Mat& frame,
                   float scale, int padX, int padY, MiniFigDirectionResult& result);

  /**
   * 推論を実行する
   */
  std::vector<std::vector<float>> infer(const cv::Mat& inputImage);
};

#endif  // MINIFIG_DIRECTION_DETECTOR_H