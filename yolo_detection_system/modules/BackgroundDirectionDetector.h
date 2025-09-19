/**
 * @file   BackgroundDirectionDetector.h
 * @brief  風景の向きを判定する画像処理クラス
 * @author Hara1274 takahashitom
 */

#ifndef BACKGROUND_DIRECTION_DETECTOR_H
#define BACKGROUND_DIRECTION_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <onnxruntime_cxx_api.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#define MODEL_INPUT_SIZE 640       // モデルの入力画像サイズ（640x640）
#define CONFIDENCE_THRESHOLD 0.5f  // 検出結果を採用する最低信頼度の閾値
#define NMS_THRESHOLD 0.5f         // 検出ボックス同士の重なりを判断する閾値

class BackgroundDirectionDetector {
 public:
  /**
   * コンストラクタ
   * @param YOLOモデルのパス
   */
  BackgroundDirectionDetector(const std::string& modelPath
                              = "../datafiles/models/11n_100epoch_&_640imgsz_background.onnx");

  /**
   * @brief 入力画像から風景の向きを判定し、結果を result に格納する
   */
  void detect();

 private:
  Ort::Env env;                          // ONNX Runtime 環境
  Ort::Session session;                  // 推論セッション
  std::vector<std::string> inputNames;   // モデルの入力名
  std::vector<std::string> outputNames;  // モデルの出力名
  const std::string inputImagePath
      = "../datafiles/detection_target/background.JPEG";  // 判定用画像のパス
  const std::string outputImagePath
      = "../datafiles/processed_images/"
        "background_detected_result.jpg";  // バウンディングボックス付きの画像パス
  const std::string outputJsonPath
      = "result/background_result.json";  // 風景の向きを判定した結果を出力するファイルパス

  /**
   * @brief         入力画像を640x640にリサイズ＆パディングする前処理関数
   * @param frame   入力画像
   * @param scale   スケール係数
   * @param padX    X方向のパディング量
   * @param padY    Y方向のパディング量
   * @return        パディング後の画像
   */
  cv::Mat prepareInputFrame(const cv::Mat& frame, float scale, int padX, int padY);

  /**
   * @brief         出力結果を後処理して検出結果を生成する関数
   * @param outputs ネットワークの出力結果
   * @param frame   入力画像フレーム
   * @param scale   スケール係数
   * @param padX    X方向のパディング量
   * @param padY    Y方向のパディング量
   */
  void analyzeDetections(const std::vector<std::vector<float>>& outputs, const cv::Mat& frame,
                         float scale, int padX, int padY);

  /**
   * @brief             推論を実行する
   * @param inputImage  入力画像フレーム
   * @return            推論結果
   */
  std::vector<std::vector<float>> infer(const cv::Mat& inputImage);

  /**
   * @brief エラー時にファイル出力を行う
   */
  void outputError();
};

#endif  // BACKGROUND_DIRECTION_DETECTOR_H