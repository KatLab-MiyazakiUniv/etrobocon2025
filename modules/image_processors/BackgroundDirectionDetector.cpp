/**
 * @file   BackgroundDirectionDetector.cpp
 * @brief  風景の向きを判定のするクラス
 * @author Hara1274
 */

#include "BackgroundDirectionDetector.h"

using namespace cv;
using namespace dnn;
using namespace std;

BackgroundDirectionDetector::BackgroundDirectionDetector(const string& modelPath)
{
  net = readNetFromONNX(modelPath);              // モデルのパスを設定する
  net.setPreferableBackend(DNN_BACKEND_OPENCV);  // OpenCVバックエンドを使用
  net.setPreferableTarget(DNN_TARGET_CPU);       // CPUをターゲットに設定
}

void BackgroundDirectionDetector::detect(const Mat& frame, BackgroundDirectionResult& result)
{
  // モデルが正しく読み込まれているかチェック
  if(net.empty()) {
    cerr << "モデルの読み込みに失敗しました！" << endl;
    result.wasDetected = false;
    return;
  }

  // 入力画像が空かどうかチェック
  if(frame.empty()) {
    cerr << "フレームが存在しません" << endl;
    result.wasDetected = false;
    return;
  }

  // 前処理パラメータを計算
  const float scale = min(MODEL_INPUT_SIZE / static_cast<float>(frame.cols),
                          MODEL_INPUT_SIZE / static_cast<float>(frame.rows));
  const int padX = (MODEL_INPUT_SIZE - static_cast<int>(frame.cols * scale)) / 2;
  const int padY = (MODEL_INPUT_SIZE - static_cast<int>(frame.rows * scale)) / 2;

  // 前処理で入力画像を640x640にリサイズ＆パディングする
  Mat inputBlob = preprocess(frame, scale, padX, padY);

  // ネットワークに入力をセット
  net.setInput(inputBlob);

  // 推論を実行し、出力を受け取る
  vector<Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());

  // 後処理で出力結果を検出結果を生成する
  postprocess(outputs, frame, scale, padX, padY, result);
}

Mat BackgroundDirectionDetector::preprocess(const Mat& frame, float scale, int padX, int padY)
{
  // リサイズ後のサイズ
  int new_w = static_cast<int>(frame.cols * scale);
  int new_h = static_cast<int>(frame.rows * scale);

  // 640×640ピクセルの背景が灰色の空画像を作り、リサイズしたものを中央に貼り付ける
  Mat output(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, frame.type(), Scalar(114, 114, 114));
  resize(frame, output(Rect(padX, padY, new_w, new_h)), Size(new_w, new_h));

  // YOLO用に画像を正規化・RGB変換
  blobFromImage(output, output, 1.0 / 255.0, Size(), Scalar(), true, false);

  return output;
}

void BackgroundDirectionDetector::postprocess(const vector<Mat>& outputs, const Mat& frame,
                                              float scale, int padX, int padY,
                                              BackgroundDirectionResult& result)
{
  vector<int> classIds;
  vector<float> confidences;
  vector<Rect> boxes;

  const float confThreshold = CONFIDENCE_THRESHOLD;
  const float nmsThreshold = NMS_THRESHOLD;

  for(const auto& output : outputs) {
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
            // 正規化座標を元画像座標に正しく変換
            float cx_norm = data[idx + 0];
            float cy_norm = data[idx + 1];
            float w_norm = data[idx + 2];
            float h_norm = data[idx + 3];

            // 640x640空間での座標 → パディング除去 → スケール逆変換
            int centerX = static_cast<int>((cx_norm * MODEL_INPUT_SIZE - padX) / scale);
            int centerY = static_cast<int>((cy_norm * MODEL_INPUT_SIZE - padY) / scale);
            int width = static_cast<int>((w_norm * MODEL_INPUT_SIZE) / scale);
            int height = static_cast<int>((h_norm * MODEL_INPUT_SIZE) / scale);

            int left = centerX - width / 2;
            int top = centerY - height / 2;

            // 境界チェック
            left = max(0, min(left, frame.cols - 1));
            top = max(0, min(top, frame.rows - 1));
            width = min(width, frame.cols - left);
            height = min(height, frame.rows - top);

            boxes.emplace_back(left, top, width, height);
            confidences.push_back(static_cast<float>(maxScore));
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

  vector<int> indices;
  NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

  if(indices.empty()) {
    result.wasDetected = false;
    return;
  }

  int bestIdx = indices[0];
  if(classIds[bestIdx] == 0) {
    result.direction = BackgroundDirection::FRONT;
  } else if(classIds[bestIdx] == 1) {
    result.direction = BackgroundDirection::BACK;
  } else if(classIds[bestIdx] == 2) {
    result.direction = BackgroundDirection::RIGHT;
  } else if(classIds[bestIdx] == 3) {
    result.direction = BackgroundDirection::LEFT;
  }
  result.wasDetected = true;

  // デバッグ用: 検出結果を画像に描画して保存
  Mat outputImage = frame.clone();
  for(size_t i = 0; i < indices.size(); ++i) {
    int idx = indices[i];
    rectangle(outputImage, boxes[idx], Scalar(0, 255, 0), 2);
    string label = to_string(classIds[idx]);
    putText(outputImage, label, boxes[idx].tl(), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);
  }
  imwrite("debug_detection.jpg", outputImage);

  // 検出された方向クラスIDを表示
  cout << "検出された方向クラスID: " << classIds[bestIdx] << endl;
}