/**
 * @file   ImageUploader.h
 * @brief  無線通信デバイスへ画像をアップロードするクラス
 * @author Hara1274
 */

#ifndef IMAGE_UPLOADER_H_
#define IMAGE_UPLOADER_H_

#include "CommandExecutor.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>

class ImageUploader {
 public:
  /**
   * @brief 画像をサーバーにアップロードする
   * @param  filePath　送信する画像のファイルパス
   * @param  uploadFileName　アップロード時のファイル名
   * @param　maxAttempts　送信試行回数の上限
   * @return アップロード成功時true、失敗時false
   */
  bool static uploadImage(const std::string& filePath, const std::string& uploadFileName,
                          int maxAttempts = 3);

  /**
   * @brief ミニフィグ画像をサーバーの/minifig/detectエンドポイントにアップロードする
   * @param  filePath　送信する画像のファイルパス
   * @param  uploadFileName　アップロード時のファイル名
   * @param　maxAttempts　送信試行回数の上限
   * @return アップロード成功時true、失敗時false
   */
  bool static uploadMiniFigImage(const std::string& filePath, const std::string& uploadFileName,
                                 int maxAttempts = 3);

 private:
  ImageUploader();
};

#endif  // IMAGE_UPLOADER_H_
