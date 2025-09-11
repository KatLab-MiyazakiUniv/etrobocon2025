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
    std::cerr << "エラー: ファイルパスが空です" << std::endl;
    return false;
  }

  // 空のファイル名のチェック
  if(uploadFileName.empty()) {
    std::cerr << "エラー: アップロード用ファイル名が空です" << std::endl;
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
    std::cerr << "エラー: ファイルが存在しません: " << fullImagePath << std::endl;
    return false;
  }

  // パスを絶対パスに変換
  std::string absolutePath = std::filesystem::absolute(fullImagePath).string();

  // 無効な最大試行回数のチェック
  if(maxAttempts <= 0) {
    std::cerr << "エラー: 無効な再試行回数: " << maxAttempts << std::endl;
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

  std::cerr << "アップロードが" << maxAttempts << "回の試行後に失敗しました" << std::endl;
  return false;
}

bool ImageUploader::uploadMiniFigImage(const std::string& filePath,
                                       const std::string& uploadFileName, int maxAttempts)
{
  // 空のファイルパスのチェック
  if(filePath.empty()) {
    std::cerr << "エラー: ファイルパスが空です" << std::endl;
    return false;
  }

  // 空のファイル名のチェック
  if(uploadFileName.empty()) {
    std::cerr << "エラー: アップロード用ファイル名が空です" << std::endl;
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
    std::cerr << "エラー: ファイルが存在しません: " << fullImagePath << std::endl;
    return false;
  }

  // パスを絶対パスに変換
  std::string absolutePath = std::filesystem::absolute(fullImagePath).string();

  // 無効な最大試行回数のチェック
  if(maxAttempts <= 0) {
    std::cerr << "エラー: 無効な再試行回数: " << maxAttempts << std::endl;
    return false;
  }

  // 試行回数(attempts)が最大試行回数(maxAttempts)を超えるまで送信を試みる
  int attempts = 0;
  while(attempts < maxAttempts) {
    std::string command = "make -C etrobocon2025 upload-minifig-image FILE_PATH=" + absolutePath;
    int result = CommandExecutor::exec(command);
    if(result == 0) {
      return true;
    }
    attempts++;
  }

  std::cerr << "ミニフィグ画像のアップロードが" << maxAttempts << "回の試行後に失敗しました"
            << std::endl;
  return false;
}