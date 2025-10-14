/**
 * @file   create_video.cpp
 * @brief  ライントレース画像から動画を作成するツール
 * @author Hara1274
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

int main(int argc, char* argv[])
{
  if(argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_dir> <output_video>" << std::endl;
    return 1;
  }

  std::string inputDir = argv[1];
  std::string outputPath = argv[2];

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
  cv::Size frameSize(firstFrame.cols * 0.75, firstFrame.rows * 0.75);
  std::cerr << "Original size: " << firstFrame.cols << "x" << firstFrame.rows << std::endl;
  std::cerr << "Video size: " << frameSize.width << "x" << frameSize.height << std::endl;

  // VideoWriterを作成
  double fps = 15.0;
  cv::VideoWriter writer(outputPath, cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, frameSize);
  if(!writer.isOpened()) {
    std::cerr << "Failed to open VideoWriter: " << outputPath << std::endl;
    return 1;
  }

  // ★ ここで「最大2分（=120秒）」の制限を設定
  const int maxFrames = static_cast<int>(fps * 120);  // 20fps × 120秒 = 2400フレーム
  int frameCount = 0;

  // 各画像を処理
  for(const auto& imagePath : imageFiles) {
    if(frameCount >= maxFrames) {
      std::cerr << "Reached 2-minute limit (" << maxFrames << " frames)." << std::endl;
      break;
    }

    cv::Mat frame = cv::imread(imagePath.string());
    if(frame.empty()) {
      std::cerr << "Failed to read image: " << imagePath << std::endl;
      continue;
    }

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
        continue;
      }

      // ROI矩形を描画（赤色、太さ2）
      cv::rectangle(frame, cv::Rect(x, y, w, h), cv::Scalar(0, 0, 255), 2);
    }

    // リサイズ
    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, frameSize, 0, 0, cv::INTER_AREA);

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
