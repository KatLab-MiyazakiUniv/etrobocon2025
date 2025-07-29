/**
 * @file   ImageUploader.cpp
 * @brief  無線通信デバイスへ画像をアップロードするクラス
 * @author Hara1274
 */

#include "ImageUploader.h"

ImageUploader::ImageUploader() {}

bool ImageUploader::uploadImage(const std::string& imagePath, int retries)
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

  // 無効な試行回数のチェック
  if(retries <= 0) {
    std::cerr << "Error: Invalid retry count: " << retries << std::endl;
    return false;
  }

  int attempts = 0;
  while(attempts < retries) {
    std::string command = "make upload-image FILE_PATH=" + imagePath;

    int result = std::system(command.c_str());
    if(result == 0) {
      return true;
    }
    attempts++;
  }

  std::cerr << "Upload failed after " << retries << " attempts" << std::endl;
  return false;
}