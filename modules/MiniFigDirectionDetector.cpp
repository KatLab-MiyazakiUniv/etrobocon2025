/**
 * @file   MinifigDirectionDetector.cpp
 * @brief  ミニフィグの向きを検出するクラス
 * @author nishijima515
 */

#include "MiniFigDirectionDetector.h"

using namespace cv;
using namespace dnn;

MiniFigDirectionDetector::MiniFigDirectionDetector(const std::string& modelPath)
{
  // コンストラクタの実装
  // ネットワークの読み込み
  net = readNetFromONNX(modelPath);  // モデルのパスを設定する
  if(net.empty()) {
    std::cerr << "モデルの読み込みに失敗しました！" << std::endl;
  } else {
    std::cout << "モデルが正常に読み込まれました。" << std::endl;
  }
  net.setPreferableBackend(DNN_BACKEND_OPENCV);  // OpenCVバックエンドを使用
  net.setPreferableTarget(DNN_TARGET_CPU);       // CPUをターゲットに設定
}

MiniFigDirectionResult MiniFigDirectionDetector::detect(const cv::Mat& frame)
{
  MiniFigDirectionResult result;

  // 入力画像の前処理
  Mat blob = preprocess(frame);
  net.setInput(blob);

  // ネットワークの推論を実行
  std::vector<Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());

  // 出力結果の後処理
  postprocess(outputs, frame, result);

  return result;
}

// 入力画像の前処理を行う関数
cv::Mat MiniFigDirectionDetector::preprocess(const cv::Mat& frame)
{
  cv::Mat blob;
  blobFromImage(frame, blob, 1.0 / 255.0, Size(640, 640), Scalar(), true, false);
  return blob;
}

// 出力結果を後処理して検出結果を生成する関数
void MiniFigDirectionDetector::postprocess(const std::vector<cv::Mat>& outputs,
                                           const cv::Mat& frame, MiniFigDirectionResult& result)
{
  std::vector<int> classIds;
  std::vector<float> confidences;
  std::vector<Rect> boxes;

  const float confThreshold = CONFIDENCE_THRESHOLD;
  const float nmsThreshold = NMS_THRESHOLD;

  for(const auto& output : outputs) {
    if(output.dims != 5) continue;

    int numAnchors = output.size[1];
    int gridHeight = output.size[2];
    int gridWidth = output.size[3];
    int attributes = output.size[4];
    const float* data = (float*)output.data;

    for(int y = 0; y < gridHeight; ++y) {
      for(int x = 0; x < gridWidth; ++x) {
        for(int anchor = 0; anchor < numAnchors; ++anchor) {
          int idx = ((anchor * gridHeight + y) * gridWidth + x) * attributes;

          float conf = data[idx + 4];
          if(conf < confThreshold) continue;

          float* classScores = (float*)&data[idx + 5];
          Mat scores(1, attributes - 5, CV_32FC1, classScores);
          Point classIdPoint;
          double maxScore;
          minMaxLoc(scores, 0, &maxScore, 0, &classIdPoint);

          if(maxScore > confThreshold) {
            float cx = data[idx + 0];
            float cy = data[idx + 1];
            float w = data[idx + 2];
            float h = data[idx + 3];

            int centerX = int(cx * frame.cols);
            int centerY = int(cy * frame.rows);
            int width = int(w * frame.cols);
            int height = int(h * frame.rows);
            int left = centerX - width / 2;
            int top = centerY - height / 2;

            boxes.emplace_back(left, top, width, height);
            confidences.push_back(maxScore);
            classIds.push_back(classIdPoint.x);
          }
        }
      }
    }
  }

  if(boxes.empty()) {
    result.wasDetected = false;
    return;
  }

  std::vector<int> indices;
  NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

  if(indices.empty()) {
    result.wasDetected = false;
    return;
  }

  int bestIdx = indices[0];

  int direction;
  if(classIds[bestIdx] == 0) {
    direction = 0;
  } else if(classIds[bestIdx] == 1) {
    direction = 2;
  } else if(classIds[bestIdx] == 2) {
    direction = 3;
  } else if(classIds[bestIdx] == 3) {
    direction = 1;
  }

  result.wasDetected = true;
  result.direction = static_cast<MiniFigDirection>(direction);
  // デバッグ出力を追加
  std::cout << "検出された方向クラスID: " << direction << std::endl;
}