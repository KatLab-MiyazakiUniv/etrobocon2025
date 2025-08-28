/**
 * @file   BackgroundDirectionDetector.cpp
 * @brief  風景の向きを判定する画像処理クラス
 * @author Hara1274 takahashitom
 */

#include "BackgroundDirectionDetector.h"

using namespace cv;
using namespace dnn;
using namespace std;
using json = nlohmann::json;

BackgroundDirectionDetector::BackgroundDirectionDetector(const string& modelPath)
  : env(ORT_LOGGING_LEVEL_WARNING, "BackgroundDirectionDetector"), session(nullptr)
{
  // セッション設定
  Ort::SessionOptions sessionOptions;
  sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

  // モデル読み込み
  session = Ort::Session(env, modelPath.c_str(), sessionOptions);

  // 入出力名を取得
  inputNames = session.GetInputNames();
  outputNames = session.GetOutputNames();
}

void BackgroundDirectionDetector::detect()
{
  // モデルが正しく読み込まれているかチェック
  if(session == nullptr) {
    cerr << "モデルが読みこまれていません" << endl;
    outputError();
    return;
  }

  // 入力画像を読み込み
  Mat frame = imread(inputImagePath);

  // 入力画像が空かどうかチェック
  if(frame.empty()) {
    cerr << "フレームが存在しません" << endl;
    outputError();
    return;
  }

  // 前処理パラメータを計算
  const float scale = min(MODEL_INPUT_SIZE / static_cast<float>(frame.cols),
                          MODEL_INPUT_SIZE / static_cast<float>(frame.rows));
  const int padX = (MODEL_INPUT_SIZE - static_cast<int>(frame.cols * scale)) / 2;
  const int padY = (MODEL_INPUT_SIZE - static_cast<int>(frame.rows * scale)) / 2;

  // 前処理で入力画像を640x640にリサイズ＆パディングする
  Mat inputBlob = prepareInputFrame(frame, scale, padX, padY);

  // 推論
  auto outputs = infer(inputBlob);

  // 後処理
  analyzeDetections(outputs, frame, scale, padX, padY);
}

Mat BackgroundDirectionDetector::prepareInputFrame(const Mat& frame, float scale, int padX,
                                                   int padY)
{
  // リサイズ後のサイズ
  int newWidth = static_cast<int>(frame.cols * scale);
  int newHeight = static_cast<int>(frame.rows * scale);

  // 640×640ピクセルの背景が灰色の空画像を作り、リサイズしたものを中央に貼り付ける
  Mat output(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, frame.type(), Scalar(114, 114, 114));
  resize(frame, output(Rect(padX, padY, newWidth, newHeight)), Size(newWidth, newHeight));

  // YOLO用に画像を正規化・RGB変換
  // RGB 変換
  cvtColor(output, output, COLOR_BGR2RGB);

  // float化＆正規化
  output.convertTo(output, CV_32F, 1.0 / 255.0);

  // CHW 化（[H,W,C] → [C,H,W]）
  vector<Mat> chw;
  split(output, chw);  // 3つのチャンネルに分割
  Mat blob;
  vconcat(chw, blob);  // 縦方向に結合して [3,H,W] の形にする

  return blob;
}

vector<vector<float>> BackgroundDirectionDetector::infer(const Mat& inputImage)
{
  // ONNX Runtime で推論を行うために、引数の型を適した形に変換する
  size_t inputTensorSize = MODEL_INPUT_SIZE * MODEL_INPUT_SIZE * 3;
  vector<float> inputTensorValues(inputTensorSize);
  memcpy(inputTensorValues.data(), inputImage.data, inputTensorSize * sizeof(float));

  array<int64_t, 4> inputShape = { 1, 3, MODEL_INPUT_SIZE, MODEL_INPUT_SIZE };

  Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

  Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
      memoryInfo, inputTensorValues.data(), inputTensorSize, inputShape.data(), inputShape.size());

  vector<const char*> inputNamesCStr;
  inputNamesCStr.reserve(inputNames.size());
  for(auto& s : inputNames) inputNamesCStr.push_back(s.c_str());

  vector<const char*> outputNamesCStr;
  outputNamesCStr.reserve(outputNames.size());
  for(auto& s : outputNames) outputNamesCStr.push_back(s.c_str());

  // 推論
  auto outputTensors = session.Run(Ort::RunOptions{ nullptr }, inputNamesCStr.data(), &inputTensor,
                                   1, outputNamesCStr.data(), outputNamesCStr.size());

  // 推論の結果をfloatのベクター配列に変換
  vector<vector<float>> results;
  results.reserve(outputTensors.size());

  for(auto& tensor : outputTensors) {
    float* floatArray = tensor.GetTensorMutableData<float>();
    size_t totalSize = tensor.GetTensorTypeAndShapeInfo().GetElementCount();
    results.emplace_back(floatArray, floatArray + totalSize);
  }

  return results;
}

void BackgroundDirectionDetector::analyzeDetections(const vector<vector<float>>& outputs,
                                                    const Mat& frame, float scale, int padX,
                                                    int padY)
{
  vector<int> classIds;       // 最も高いスコアを持つクラスIDを格納するリスト
  vector<float> confidences;  // 信頼度を格納するリスト
  vector<Rect> boxes;         // バウンディングボックスを格納するリスト

  if(outputs.empty()) {
    outputError();
    return;
  }

  const vector<float>& data = outputs[0];

  // YOLO11 出力 shape: [1, 8, N]
  int attributes = 8;
  int numClasses = 8 - 4;  // 座標4つ以外はクラススコア
  int numBoxes = data.size() / attributes;

  // 検出候補の数だけループを回す
  for(int i = 0; i < numBoxes; i++) {
    // クラススコアの最大値とクラスIDを取得
    float maxScore = -1.0f;
    int bestClass = -1;
    for(int j = 0; j < numClasses; j++) {
      float score = data[i + numBoxes * (4 + j)];
      if(score > maxScore) {
        maxScore = score;
        bestClass = j;
      }
    }

    // 最大クラススコアが閾値を超えているかチェック
    if(maxScore < CONFIDENCE_THRESHOLD) continue;

    float cx = data[i];
    float cy = data[i + numBoxes];
    float w = data[i + numBoxes * 2];
    float h = data[i + numBoxes * 3];

    // バウンディングボックスの中心座標とサイズを表す値
    int centerX = static_cast<int>((cx - padX) / scale);
    int centerY = static_cast<int>((cy - padY) / scale);
    int width = static_cast<int>(w / scale);
    int height = static_cast<int>(h / scale);

    int left = centerX - width / 2;
    int top = centerY - height / 2;

    // 画面内に収まるように処理
    left = max(0, min(left, frame.cols - 1));
    top = max(0, min(top, frame.rows - 1));
    width = min(width, frame.cols - left);
    height = min(height, frame.rows - top);

    // リストに追加
    boxes.emplace_back(left, top, width, height);
    confidences.push_back(maxScore);
    classIds.push_back(bestClass);
  }

  // 信頼度を超えるバウンディングボックスがなかった時の処理
  if(boxes.empty()) {
    outputError();
    return;
  }

  // Non-Maximum Suppression (NMS) を実行し、重複する検出ボックスを削減する
  vector<int> indices;
  NMSBoxes(boxes, confidences, CONFIDENCE_THRESHOLD, NMS_THRESHOLD, indices);

  // NMS の結果、検出がなければ処理を終了
  if(indices.empty()) {
    outputError();
    return;
  }

  // NMS 後の最良の検出ボックスのインデックスを取得
  int bestIdx = indices[0];
  string direction;

  // 最良検出ボックスのクラスIDに応じて方向を設定
  if(classIds[bestIdx] == 0) {
    direction = "FRONT";
  } else if(classIds[bestIdx] == 1) {
    direction = "BACK";
  } else if(classIds[bestIdx] == 2) {
    direction = "RIGHT";
  } else if(classIds[bestIdx] == 3) {
    direction = "LEFT";
  }

  // JSON出力
  json j;
  j["wasDetected"] = true;
  j["direction"] = direction;
  ofstream(outputJsonPath) << j.dump(4);

  // デバッグ用: 検出結果を画像に描画して保存
  Mat outputImage = frame.clone();
  for(size_t i = 0; i < indices.size(); ++i) {
    int idx = indices[i];
    rectangle(outputImage, boxes[idx], Scalar(0, 255, 0), 2);
    string label = to_string(classIds[idx]);
    putText(outputImage, label, boxes[idx].tl(), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);
  }
  imwrite(outputImagePath, outputImage);

  // 検出された方向クラスIDを表示
  cout << "検出された方向クラスID: " << classIds[bestIdx] << endl;
}

void BackgroundDirectionDetector::outputError()
{
  // JSON出力
  json j;
  j["wasDetected"] = false;
  j["direction"] = "NONE";
  ofstream(outputJsonPath) << j.dump(4);
  return;
}