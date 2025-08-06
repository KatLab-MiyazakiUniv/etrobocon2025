/**
 * @file   ImageUploader.cpp
 * @brief  無線通信デバイスへ画像をアップロードするクラス
 * @author Hara1274
 */

#include "ImageUploader.h"

ImageUploader::ImageUploader() {}

bool ImageUploader::uploadImage(const std::string& filePath, const std::string& uploadFileName,
                                int maxAttempts)
{
  // 空のファイルパスのチェック
  if(filePath.empty()) {
    std::cerr << "Error: Empty file path provided" << std::endl;
    return false;
  }

  // 空のファイル名のチェック
  if(uploadFileName.empty()) {
    std::cerr << "Error: Empty upload file name provided" << std::endl;
    return false;
  }

  // 拡張子がない場合は.JPEGを追加
  std::string processedFileName = uploadFileName;
  if(!uploadFileName.empty() && std::filesystem::path(uploadFileName).extension().empty()) {
    processedFileName += ".JPEG";
  }

  // フルパスを作成
  std::string fullImagePath = filePath + "/" + processedFileName;

  // ファイル存在チェック
  std::ifstream file(fullImagePath);
  if(!file.good()) {
    std::cerr << "Error: File does not exist: " << fullImagePath << std::endl;
    return false;
  }

  // パスを絶対パスに変換
  std::string absolutePath = std::filesystem::absolute(fullImagePath).string();

  // 無効な最大試行回数のチェック
  if(maxAttempts <= 0) {
    std::cerr << "Error: Invalid retry count: " << maxAttempts << std::endl;
    return false;
  }

  // 試行回数(attempts)が最大試行回数(maxAttempts)を超えるまで送信を試みる
  int attempts = 0;
  while(attempts < maxAttempts) {
    std::string command = "make -C etrobocon2025 upload-image FILE_PATH=" + absolutePath;
    int result = CommandExecutor::exec(command);
    if(result == 0) {
      return true;
    }
    attempts++;
  }

  std::cerr << "Upload failed after " << maxAttempts << " attempts" << std::endl;
  return false;
}