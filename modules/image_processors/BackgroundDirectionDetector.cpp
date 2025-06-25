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
  int newWidth = static_cast<int>(frame.cols * scale);
  int newHeight = static_cast<int>(frame.rows * scale);

  // 640×640ピクセルの背景が灰色の空画像を作り、リサイズしたものを中央に貼り付ける
  Mat output(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, frame.type(), Scalar(114, 114, 114));
  resize(frame, output(Rect(padX, padY, newWidth, newHeight)), Size(newWidth, newHeight));

  // YOLO用に画像を正規化・RGB変換
  Mat blob;
  blobFromImage(output, blob, 1.0 / 255.0, Size(), Scalar(), true, false);

  return blob;
}

void BackgroundDirectionDetector::postprocess(const vector<Mat>& outputs, const Mat& frame,
                                              float scale, int padX, int padY,
                                              BackgroundDirectionResult& result)
{
  vector<int> classIds;       // 最も高いスコアを持つクラスIDを格納するリスト
  vector<float> confidences;  // 信頼度を格納するリスト
  vector<Rect> boxes;         // バウンディングボックスを格納するリスト

  for(const auto& output : outputs) {
    int numBoxes = output.size[1];    // 検出候補の総数
    int attributes = output.size[2];  // ボックスが属性の数　（座標やスコアなど）

    const float* data = (float*)output.data;  // 出力の実体をdataに格納する

    // 検出候補の数だけループを回す
    for(int i = 0; i < numBoxes; ++i) {
      // 処理中の候補の先頭のデータ位置を計算
      int idx = i * attributes;

      // 5番目にあるボックスの信頼度（confidence）スコアを取得。頼度が閾値未満なら次の候補へ
      float conf = data[idx + 4];
      if(conf < CONFIDENCE_THRESHOLD) continue;

      // 最も高いクラススコアとそのクラスID（インデックス）を取得
      float* classScores = (float*)&data[idx + 5];
      Mat scores(1, attributes - 5, CV_32FC1, classScores);
      Point classIdPoint;
      double maxScore;
      minMaxLoc(scores, 0, &maxScore, 0, &classIdPoint);

      // 最大クラススコアも閾値を超えているかチェック
      if(maxScore > CONFIDENCE_THRESHOLD) {
        // バウンディングボックスの中心座標とサイズを表す値
        float cx_norm = data[idx + 0];
        float cy_norm = data[idx + 1];
        float w_norm = data[idx + 2];
        float h_norm = data[idx + 3];

        int centerX = static_cast<int>((cx_norm - padX) / scale);
        int centerY = static_cast<int>((cy_norm - padY) / scale);
        int width = static_cast<int>(w_norm / scale);
        int height = static_cast<int>(h_norm / scale);

        int left = centerX - width / 2;
        int top = centerY - height / 2;

        // 画面内に収まるように処理
        left = max(0, min(left, frame.cols - 1));
        top = max(0, min(top, frame.rows - 1));
        width = min(width, frame.cols - left);
        height = min(height, frame.rows - top);

        // リストに追加
        boxes.emplace_back(left, top, width, height);
        confidences.push_back(static_cast<float>(maxScore));
        classIds.push_back(classIdPoint.x);
      }
    }
  }

  // 信頼度を超えるバウンディングボックスがなかった時の処理
  if(boxes.empty()) {
    result.wasDetected = false;
    return;
  }

  // Non-Maximum Suppression (NMS) を実行し、重複する検出ボックスを削減する
  vector<int> indices;
  NMSBoxes(boxes, confidences, CONFIDENCE_THRESHOLD, NMS_THRESHOLD, indices);

  // NMS の結果、検出がなければ処理を終了
  if(indices.empty()) {
    result.wasDetected = false;
    return;
  }

  // NMS 後の最良の検出ボックスのインデックスを取得
  int bestIdx = indices[0];

  // 最良検出ボックスのクラスIDに応じて方向を設定
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
  imwrite("etrobocon2025/datafiles/snapshots/background_result.JPEG", outputImage);

  // 検出された方向クラスIDを表示
  cout << "検出された方向クラスID: " << classIds[bestIdx] << endl;
}