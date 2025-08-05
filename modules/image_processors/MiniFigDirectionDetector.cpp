/**
 * @file   MiniFigDirectionDetector.cpp
 * @brief  ミニフィグの向きを検出するクラス
 * @author nishijima515
 */

#include "MiniFigDirectionDetector.h"

cv::dnn::Net MiniFigDirectionDetector::net;
bool MiniFigDirectionDetector::isInitialized = false;

void MiniFigDirectionDetector::initializeModel(const std::string& modelPath)
{
  if(isInitialized) return;

  // モデルの読み込み
  net = cv::dnn::readNetFromONNX(modelPath);
  if(net.empty()) {
    std::cerr << "モデルの読み込みに失敗しました！" << std::endl;
    return;
  } else {
    std::cout << "モデルが正常に読み込まれました。" << std::endl;
  }
  net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);  // OpenCVバックエンドを使用
  net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);       // CPUをターゲットに設定

  isInitialized = true;
}

MiniFigDirectionDetector::MiniFigDirectionDetector() {}

void MiniFigDirectionDetector::detect(const cv::Mat& frame, MiniFigDirectionResult& result)
{
  // モデルが正しく読み込まれているかチェック
  if(net.empty()) {
    std::cerr << "モデルが読み込まれていません" << std::endl;
    result.wasDetected = false;
    return;
  }

  // 入力画像が空かどうかチェック
  if(frame.empty()) {
    std::cerr << "フレームが存在しません" << std::endl;
    result.wasDetected = false;
    return;
  }

  // 前処理パラメータを計算
  const float scale = std::min(MODEL_INPUT_SIZE / static_cast<float>(frame.cols),
                               MODEL_INPUT_SIZE / static_cast<float>(frame.rows));
  const int padX = (MODEL_INPUT_SIZE - static_cast<int>(frame.cols * scale)) / 2;
  const int padY = (MODEL_INPUT_SIZE - static_cast<int>(frame.rows * scale)) / 2;

  // 入力画像の前処理
  cv::Mat blob = preprocess(frame, scale, padX, padY);
  net.setInput(blob);

  // ネットワークの推論を実行
  std::vector<cv::Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());

  // 出力結果の後処理
  postprocess(outputs, frame, scale, padX, padY, result);
}

// 入力画像の前処理を行う関数
// 画像をモデルに合うサイズ(640x640）にリサイズし、画素値を0~1に正規化する
cv::Mat MiniFigDirectionDetector::preprocess(const cv::Mat& frame, float scale, int padX, int padY)
{
  // リサイズ後のサイズ
  int newWidth = static_cast<int>(frame.cols * scale);
  int newHeight = static_cast<int>(frame.rows * scale);

  // 640×640ピクセルの背景が灰色の空画像を作り、リサイズしたものを中央に貼り付ける
  cv::Mat output(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, frame.type(), cv::Scalar(114, 114, 114));
  cv::resize(frame, output(cv::Rect(padX, padY, newWidth, newHeight)),
             cv::Size(newWidth, newHeight));

  cv::Mat blob;
  cv::dnn::blobFromImage(output, blob, 1.0 / 255.0, cv::Size(), cv::Scalar(), true, false);
  return blob;
}

// 出力結果を後処理して検出結果を生成する関数
void MiniFigDirectionDetector::postprocess(const std::vector<cv::Mat>& outputs,
                                           const cv::Mat& frame, float scale, int padX, int padY,
                                           MiniFigDirectionResult& result)
{
  // 推論結果からの抽出用データ構造を初期化
  std::vector<int> classIds;
  std::vector<float> confidences;
  std::vector<cv::Rect> boxes;

  const float confThreshold = CONFIDENCE_THRESHOLD;
  const float nmsThreshold = NMS_THRESHOLD;

  for(const cv::Mat& output : outputs) {
    // 出力outputがYOLOv5の最終的な推論結果（簡略化された形式）かどうかを判定して、それ以外の出力は無視する
    if(output.dims != 3 || output.size[1] != 25200) continue;

    int numBoxes = output.size[1];    // 検出候補の総数
    int attributes = output.size[2];  // ボックスの属性の数　（座標やスコアなど）

    const float* data = reinterpret_cast<float*>(output.data);  // 出力の実体をdataに格納する

    // 検出候補の数だけループを回す
    for(int i = 0; i < numBoxes; ++i) {
      // 処理中の候補の先頭のデータ位置を計算
      int idx = i * attributes;

      // 5番目にあるボックスの信頼度（confidence）スコアを取得。頼度が閾値未満なら次の候補へ
      float conf = data[idx + 4];
      if(conf < confThreshold) continue;

      // 最大クラススコアを算出
      float maxScore = -1.0f;
      int bestClass = -1;
      for(int j = 0; j < attributes - 5; j++) {
        float score = data[idx + 5 + j];
        if(score > maxScore) {
          maxScore = score;
          bestClass = j;
        }
      }

      // 最大クラススコアも閾値を超えているかチェック
      if(maxScore > confThreshold) {
        float cx = data[idx + 0];
        float cy = data[idx + 1];
        float w = data[idx + 2];
        float h = data[idx + 3];

        // バウンディングボックスの中心座標とサイズを表す値
        int centerX = static_cast<int>((cx - padX) / scale);
        int centerY = static_cast<int>((cy - padY) / scale);
        int width = static_cast<int>(w / scale);
        int height = static_cast<int>(h / scale);

        int left = centerX - width / 2;
        int top = centerY - height / 2;

        // 画面内に収まるように処理
        left = std::max(0, std::min(left, frame.cols - 1));
        top = std::max(0, std::min(top, frame.rows - 1));
        width = std::min(width, frame.cols - left);
        height = std::min(height, frame.rows - top);

        boxes.emplace_back(left, top, width, height);
        confidences.push_back(maxScore);
        classIds.push_back(bestClass);
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