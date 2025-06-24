/**
 * @file   BackgroundDirectionDetector.h
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
    cout << "モデルの読み込みに失敗しました！" << endl;
    result.wasDetected = false;
    return;
  }

  // 入力画像が空かどうかチェック
  if(frame.empty()) {
    cerr << "フレームが存在しません" << endl;
    result.wasDetected = false;
    return;
  }

  // 画像の前処理（リサイズ、パディング、正規化、色変換）
  Mat inputBlob = preprocess(frame);

  // ネットワークに入力をセット
  net.setInput(inputBlob);

  // 推論を実行し、出力を受け取る
  std::vector<Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());
}

Mat BackgroundDirectionDetector::preprocess(const Mat& frame)
{
  // 目的の正方形画像サイズ（640x640ピクセル）
  const int size = 640;

  // 縦横の比率の小さい方を選び、アスペクト比を維持したままリサイズするための倍率を計算
  float scale
      = std::min(size / static_cast<float>(frame.cols), size / static_cast<float>(frame.rows));

  // リサイズ後のサイズ
  int new_w = static_cast<int>(frame.cols * scale);
  int new_h = static_cast<int>(frame.rows * scale);

  // 640×640ピクセルの背景が灰色の空画像を作り、リサイズしたものを中央に貼り付ける
  Mat output(size, size, frame.type(), Scalar(114, 114, 114));
  resize(frame, output(Rect((size - new_w) / 2, (size - new_h) / 2, new_w, new_h)),
         Size(new_w, new_h));

  // YOLO用に画像を正規化・RGB変換（6番目の引数でBGR→RGB）
  dnn::blobFromImage(output, output, 1.0 / 255.0, Size(), Scalar(), true, false);

  return output;
}
