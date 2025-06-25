/**
 * @file   MiniFigDirectionDetector.cpp
 * @brief  ミニフィグの向きを検出するクラス
 * @author nishijima515
 */

#include "MiniFigDirectionDetector.h"

MiniFigDirectionDetector::MiniFigDirectionDetector(const std::string& modelPath)
  : modelPath(modelPath)
{
  // ネットワークの読み込み
  net = cv::dnn::readNetFromONNX(modelPath);  // モデルのパスを設定する
  if(net.empty()) {
    std::cerr << "モデルの読み込みに失敗しました！" << std::endl;
  } else {
    std::cout << "モデルが正常に読み込まれました。" << std::endl;
  }
  net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);  // OpenCVバックエンドを使用
  net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);       // CPUをターゲットに設定
}

void MiniFigDirectionDetector::detect(const cv::Mat& frame, MiniFigDirectionResult& result)
{
  // 入力画像の前処理
  cv::Mat blob = preprocess(frame);
  net.setInput(blob);

  // ネットワークの推論を実行
  std::vector<cv::Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());

  // 出力結果の後処理
  postprocess(outputs, frame, result);
}

// 入力画像の前処理を行う関数
// 画像をモデルに合うサイズ(640x640）にリサイズし、画素値を0~1に正規化する
cv::Mat MiniFigDirectionDetector::preprocess(const cv::Mat& frame)
{
  cv::Mat blob;
  cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0, cv::Size(640, 640), cv::Scalar(), true, false);
  return blob;
}

// 出力結果を後処理して検出結果を生成する関数
void MiniFigDirectionDetector::postprocess(const std::vector<cv::Mat>& outputs,
                                           const cv::Mat& frame, MiniFigDirectionResult& result)
{
  // 推論結果からの抽出用データ構造を初期化
  std::vector<int> classIds;
  std::vector<float> confidences;
  std::vector<cv::Rect> boxes;

  const float confThreshold = CONFIDENCE_THRESHOLD;
  const float nmsThreshold = NMS_THRESHOLD;

  // 各出力（YOLOでは複数層出力）を走査
  for(const auto& output : outputs) {
    if(output.dims != 5) continue;

    int numAnchors = output.size[1];  // 1グリッドセルあたりのアンカー数
    int gridHeight = output.size[2];  // グリッドの高さ
    int gridWidth = output.size[3];   // グリッドの幅
    int attributes = output.size[4];  // 属性数
    const float* data = (float*)output.data;

    // グリッド全体を走査(縦・横・アンカー)
    for(int y = 0; y < gridHeight; ++y) {
      for(int x = 0; x < gridWidth; ++x) {
        for(int anchor = 0; anchor < numAnchors; ++anchor) {
          // 5次元配列の中で対象データのインデックスを計算
          int idx = ((anchor * gridHeight + y) * gridWidth + x) * attributes;

          // 信頼度が閾値未満の場合はスキップ
          float conf = data[idx + 4];
          if(conf < confThreshold) continue;

          // クラススコアをMatとして取得し、最大スコアとそのクラスIDを求める
          float* classScores = (float*)&data[idx + 5];
          cv::Mat scores(1, attributes - 5, CV_32FC1, classScores);
          cv::Point classIdPoint;
          double maxScore;
          cv::minMaxLoc(scores, 0, &maxScore, 0, &classIdPoint);

          if(maxScore > confThreshold) {
            // バウンディングボックス中心座標とサイズを取得
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

  // 非最大抑制(NMS)を適用し、最も信頼度の高い検出結果を選別
  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

  if(indices.empty()) {
    result.wasDetected = false;
    return;
  }

  // 最も信頼度の高い検出結果を使って方向を決定
  int bestIdx = indices[0];

  // classIdsからミニフィグの向きを決定
  if(classIds[bestIdx] == 0) {
    result.direction = MiniFigDirection::FRONT;
  } else if(classIds[bestIdx] == 1) {
    result.direction = MiniFigDirection::BACK;
  } else if(classIds[bestIdx] == 2) {
    result.direction = MiniFigDirection::RIGHT;
  } else if(classIds[bestIdx] == 3) {
    result.direction = MiniFigDirection::LEFT;
  }

  result.wasDetected = true;

  // 検出結果を画像に描画
  cv::Mat outputImage = frame.clone();
  for(size_t i = 0; i < indices.size(); ++i) {
    int idx = indices[i];
    cv::rectangle(outputImage, boxes[idx], cv::Scalar(0, 255, 0), 2);
    std::string label = std::to_string(classIds[idx]);
    cv::putText(outputImage, label, boxes[idx].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                cv::Scalar(255, 0, 0), 1);
  }
  cv::imwrite(outputImagePath, outputImage);

  // 検出された方向クラスIDを表示
  std::cout << "検出された方向クラスID: " << static_cast<int>(result.direction) << std::endl;
}