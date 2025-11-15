/**
 * @file   create_video.cpp
 * @brief  ライントレース画像から動画を作成するツール
 * @author Hara1274 HaruArima08
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

int main(int argc, char* argv[])
{
  if(argc < 3 || argc > 6) {
    std::cerr << "Usage: " << argv[0]
              << " <input_dir> <output_video> [duration_seconds] [start_seconds] [end_seconds]"
              << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "  " << argv[0] << " input_dir output.mp4" << std::endl;
    std::cerr << "  " << argv[0] << " input_dir output.mp4 120" << std::endl;
    std::cerr << "  " << argv[0] << " input_dir output.mp4 0 60 100" << std::endl;
    return 1;
  }

  std::string inputDir = argv[1];
  std::string outputPath = argv[2];
  int durationSeconds = (argc >= 4) ? std::stoi(argv[3]) : 120;  // デフォルト120秒
  int startSeconds = (argc >= 5) ? std::stoi(argv[4]) : 0;       // デフォルト0秒から
  int endSeconds = (argc == 6) ? std::stoi(argv[5]) : -1;        // デフォルト指定なし

  namespace fs = std::filesystem;

  // 入力ディレクトリの存在確認
  if(!fs::exists(inputDir) || !fs::is_directory(inputDir)) {
    std::cerr << "Error: " << inputDir << " not found or is not a directory" << std::endl;
    return 1;
  }

  // 画像ファイルを収集してソート
  std::vector<fs::path> imageFiles;
  for(const auto& entry : fs::directory_iterator(inputDir)) {
    if(entry.is_regular_file()) {
      std::string filename = entry.path().filename().string();
      if(filename.find("roi_") == 0 && filename.find(".JPEG") != std::string::npos) {
        imageFiles.push_back(entry.path());
      }
    }
  }

  if(imageFiles.empty()) {
    std::cerr << "No image files found in " << inputDir << std::endl;
    return 1;
  }

  // ファイル名でソート（タイムスタンプ順）
  std::sort(imageFiles.begin(), imageFiles.end(), [](const fs::path& a, const fs::path& b) {
    std::string nameA = a.filename().string();
    std::string nameB = b.filename().string();

    // タイムスタンプを抽出（最後の_以降の数字）
    size_t posA = nameA.rfind('_');
    size_t posB = nameB.rfind('_');

    if(posA != std::string::npos && posB != std::string::npos) {
      long long tsA = std::stoll(nameA.substr(posA + 1));
      long long tsB = std::stoll(nameB.substr(posB + 1));
      return tsA < tsB;
    }
    return nameA < nameB;
  });

  std::cerr << "Found " << imageFiles.size() << " images" << std::endl;

  // 最初の画像を読み込んでサイズを取得
  cv::Mat firstFrame = cv::imread(imageFiles[0].string());
  if(firstFrame.empty()) {
    std::cerr << "Failed to read first image: " << imageFiles[0] << std::endl;
    return 1;
  }

  // 解像度を縮小
  double scale = 0.5;
  cv::Size frameSize(static_cast<int>(firstFrame.cols * scale),
                     static_cast<int>(firstFrame.rows * scale));
  std::cerr << "Original size: " << firstFrame.cols << "x" << firstFrame.rows << std::endl;
  std::cerr << "Video size: " << frameSize.width << "x" << frameSize.height << std::endl;

  // VideoWriterを作成
  double fps = 15.0;
  cv::VideoWriter writer(outputPath, cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, frameSize);
  if(!writer.isOpened()) {
    std::cerr << "Failed to open VideoWriter: " << outputPath << std::endl;
    return 1;
  }

  // 時間指定の処理
  double totalDuration = static_cast<double>(imageFiles.size()) / fps;

  // 終了時間が指定されていない場合は、開始時間+durationSecondsまたは全体の長さ
  if(endSeconds == -1) {
    if(startSeconds > 0) {
      endSeconds = startSeconds + durationSeconds;
    } else {
      endSeconds = std::min(durationSeconds, 120);
    }
  }

  // 範囲チェック
  if(startSeconds < 0) startSeconds = 0;
  if(endSeconds > totalDuration) endSeconds = static_cast<int>(totalDuration);
  if(startSeconds >= endSeconds) {
    std::cerr << "Error: start_seconds (" << startSeconds << ") must be less than end_seconds ("
              << endSeconds << ")" << std::endl;
    return 1;
  }

  // フレーム範囲の計算
  int startFrame = static_cast<int>(fps * startSeconds);
  int endFrame = static_cast<int>(fps * endSeconds);

  std::cerr << "Total images: " << imageFiles.size() << " (" << totalDuration << " seconds at "
            << fps << " fps)" << std::endl;
  std::cerr << "Creating video from " << startSeconds << "s to " << endSeconds << "s (frames "
            << startFrame << "-" << endFrame << ")" << std::endl;

  // 各画像を処理
  int frameCount = 0;
  for(size_t i = 0; i < imageFiles.size(); i++) {
    // 指定された時間範囲外のフレームをスキップ
    if(static_cast<int>(i) < startFrame) continue;
    if(static_cast<int>(i) >= endFrame) break;

    const auto& imagePath = imageFiles[i];

    cv::Mat frame = cv::imread(imagePath.string());
    if(frame.empty()) {
      std::cerr << "Failed to read image: " << imagePath << std::endl;
      continue;
    }

    // --- ここから変更点：リサイズを先に行い、ROIはスケール後の座標で描画 ---
    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, frameSize, 0, 0, cv::INTER_AREA);

    // ファイル名からROI情報を抽出
    std::string filename = imagePath.filename().string();
    int x = 0, y = 0, w = 0, h = 0;

    size_t xPos = filename.find("_x");
    size_t yPos = filename.find("_y");
    size_t wPos = filename.find("_w");
    size_t hPos = filename.find("_h");

    if(xPos != std::string::npos && yPos != std::string::npos && wPos != std::string::npos
       && hPos != std::string::npos) {
      try {
        x = std::stoi(filename.substr(xPos + 2, yPos - xPos - 2));
        y = std::stoi(filename.substr(yPos + 2, wPos - yPos - 2));
        w = std::stoi(filename.substr(wPos + 2, hPos - wPos - 2));
        h = std::stoi(filename.substr(hPos + 2));
      } catch(const std::exception& e) {
        std::cerr << "Failed to parse ROI from filename: " << filename << std::endl;
        // リサイズ済みフレームを書き出して次へ
        writer.write(resizedFrame);
        frameCount++;
        continue;
      }

      // スケールに合わせたROI座標
      int sx = static_cast<int>(x * scale);
      int sy = static_cast<int>(y * scale);
      int sw = static_cast<int>(w * scale);
      int sh = static_cast<int>(h * scale);

      // 範囲チェック（簡潔に調整）
      if(sx < 0) sx = 0;
      if(sy < 0) sy = 0;
      if(sw < 0) sw = 0;
      if(sh < 0) sh = 0;
      if(sx + sw > resizedFrame.cols) sw = resizedFrame.cols - sx;
      if(sy + sh > resizedFrame.rows) sh = resizedFrame.rows - sy;

      // ROI矩形を描画（赤色、太さ2）
      if(sw > 0 && sh > 0) {
        cv::rectangle(resizedFrame, cv::Rect(sx, sy, sw, sh), cv::Scalar(0, 0, 255), 2);
      }

      // ROI文字情報を描画
      std::string roiText1 = "ROI: x=" + std::to_string(x) + ", y=" + std::to_string(y);
      std::string roiText2 = "w=" + std::to_string(w) + ", h=" + std::to_string(h);
      cv::putText(resizedFrame, roiText1, cv::Point(10, 22), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                  cv::Scalar(0, 0, 255), 1);
      cv::putText(resizedFrame, roiText2, cv::Point(10, 22 + 18), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                  cv::Scalar(0, 0, 255), 1);
    }

    // 動画に書き込み
    writer.write(resizedFrame);
    frameCount++;
  }

  writer.release();

  // 元の画像ディレクトリを削除
  try {
    fs::remove_all(inputDir);
    std::cerr << "Removed directory: " << inputDir << std::endl;
  } catch(const std::exception& e) {
    std::cerr << "Failed to remove directory: " << e.what() << std::endl;
  }

  std::cerr << "動画を作成しました: " << outputPath << std::endl;

  return 0;
}