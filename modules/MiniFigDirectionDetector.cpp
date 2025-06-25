/**
 * @file   MiniFigDirectionDetector.cpp
 * @brief  ミニフィグの向きを検出するクラス
 * @author nishijima515
 */

#include "MiniFigDirectionDetector.h"

MiniFigDirectionDetector::MiniFigDirectionDetector(const std::string& modelPath,
                                                   const std::string& saveImagePath)
  : modelPath(modelPath), saveImagePath(saveImagePath)
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

void MiniFigDirectionDetector::detect(const cv::Mat& frame, /*const std::string& saveImagePath,*/
                                      MiniFigDirectionResult& result)
{
  // 入力画像の前処理
  cv::Mat blob = preprocess(frame);
  net.setInput(blob);

  // ネットワークの推論を実行
  std::vector<cv::Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());

  // 出力結果の後処理
  postprocess(outputs, frame, result /*, saveImagePath*/);
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
                                           const cv::Mat& frame, MiniFigDirectionResult& result/*,
                                           const std::string& saveImagePath*/)
{
  std::vector<int> classIds;
  std::vector<float> confidences;
  std::vector<cv::Rect> boxes;

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
          cv::Mat scores(1, attributes - 5, CV_32FC1, classScores);
          cv::Point classIdPoint;
          double maxScore;
          cv::minMaxLoc(scores, 0, &maxScore, 0, &classIdPoint);

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
  cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

  if(indices.empty()) {
    result.wasDetected = false;
    return;
  }

  int bestIdx = indices[0];

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
  cv::imwrite(saveImagePath, outputImage);

  // 検出された方向クラスIDを表示
  std::cout << "検出された方向クラスID: " << static_cast<int>(result.direction) << std::endl;
}