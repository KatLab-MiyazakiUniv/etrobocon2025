/**
 * @file   ImageUploader.cpp
 * @brief  無線通信デバイスへ画像をアップロードするクラス
 * @author Hara1274
 */

#include "ImageUploader.h"

ImageUploader::ImageUploader() {}

bool ImageUploader::uploadImage(const std::string& imagePath, int maxAttempts)
{
  // 拡張子がない場合は.JPEGを追加
  std::string processedImagePath = imagePath;
  if(!imagePath.empty() && std::filesystem::path(imagePath).extension().empty()) {
    processedImagePath += ".JPEG";
  }

  // 空のファイルパスのチェック
  if(processedImagePath.empty()) {
    std::cerr << "Error: Empty image path provided" << std::endl;
    return false;
  }

  // ファイル存在チェック
  std::ifstream file(processedImagePath);
  if(!file.good()) {
    std::cerr << "Error: File does not exist: " << processedImagePath << std::endl;
    return false;
  }

  // パスを絶対パスに変換
  std::string absolutePath = std::filesystem::absolute(processedImagePath).string();

  // 無効な最大試行回数のチェック
  if(maxAttempts <= 0) {
    std::cerr << "Error: Invalid retry count: " << maxAttempts << std::endl;
    return false;
  }

  // 試行回数(attempts)が最大試行回数(maxAttempts)を超えるまで送信を試みる
  int attempts = 0;
  while(attempts < maxAttempts) {
    std::string command = "make -C etrobocon2025 upload-image FILE_PATH=" + absolutePath;

    int result = std::system(command.c_str());
    if(result == 0) {
      return true;
    }
    attempts++;
  }

  std::cerr << "Upload failed after " << maxAttempts << " attempts" << std::endl;
  return false;
}