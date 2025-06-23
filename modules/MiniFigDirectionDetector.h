/**
 * @file   MinifigDirectionDetector.h
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

#define CONFIDENCE_THRESHOLD 0.4  // 検出結果を採用する最低信頼度の閾値
#define NMS_THRESHOLD 0.4         // 検出ボックス同士の重なりを判断する閾値

using namespace cv;
using namespace dnn;

// ミニフィグの向きを表す列挙体
enum class MiniFigDirection { FRONT, LEFT, BACK, RIGHT };

struct MiniFigDirectionResult {
  bool wasDetected = false;    // 検出が成功したかどうか
  MiniFigDirection direction;  // ミニフィグの向きを表す列挙体
};

class MiniFigDirectionDetector {
 public:
  // コンストラクタ
  MiniFigDirectionDetector(const std::string& modelPath);

  // ミニフィグの向き検出を行う処理
  // MiniFigDirectionResult 型の構造体を返す
  MiniFigDirectionResult detect(const cv::Mat& frame, const std::string& saveImagePath);

 private:
  cv::dnn::Net net;  // DNNモデルを格納する変数

  // モデルの前処理を行う関数
  cv::Mat preprocess(const cv::Mat& frame);

  /**
   * @brief 出力結果を後処理して検出結果を生成する関数
   * @param outputs: ネットワークの出力結果
   * @param frame: 入力画像フレーム
   * @param result: 検出結果を格納する構造体
   * @param saveImagePath: 検出結果を保存する画像のパス
   */
  void postprocess(const std::vector<cv::Mat>& outputs, const cv::Mat& frame,
                   MiniFigDirectionResult& result, const std::string& saveImagePath);
};

#endif  // MINIFIG_DIRECTION_DETECTOR_H