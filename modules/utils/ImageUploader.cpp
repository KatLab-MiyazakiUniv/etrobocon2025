/**
 * @file   ImageUploader.cpp
 * @brief  無線通信デバイスへ画像をアップロードするクラス
 * @author Hara1274
 */

#include "ImageUploader.h"

ImageUploader::ImageUploader() {}

bool ImageUploader::uploadImage(const std::string& imagePath, int maxAttempts)
{
  // Makefile存在チェック
  std::ifstream makefile("Makefile");
  if(!makefile.good()) {
    std::cerr << "Error: Makefile not found in current directory" << std::endl;
    return false;
  }

  // 空のファイルパスのチェック
  if(imagePath.empty()) {
    std::cerr << "Error: Empty image path provided" << std::endl;
    return false;
  }

  // ファイル存在チェック
  std::ifstream file(imagePath);
  if(!file.good()) {
    std::cerr << "Error: File does not exist: " << imagePath << std::endl;
    return false;
  }

  // 無効な最大試行回数のチェック
  if(maxAttempts <= 0) {
    std::cerr << "Error: Invalid retry count: " << maxAttempts << std::endl;
    return false;
  }

  // 試行回数(attempts)が最大試行回数(maxAttempts)を超えるまで送信を試みる
  int attempts = 0;
  while(attempts < maxAttempts) {
    std::string command = "make upload-image FILE_PATH=" + imagePath;

    int result = std::system(command.c_str());
    if(result == 0) {
      return true;
    }
    attempts++;
  }

  std::cerr << "Upload failed after " << maxAttempts << " attempts" << std::endl;
  return false;
}