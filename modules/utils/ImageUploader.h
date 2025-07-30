/**
 * @file   ImageUploader.h
 * @brief  無線通信デバイスへ画像をアップロードするクラス
 * @author Hara1274
 */

#ifndef IMAGE_UPLOADER_H_
#define IMAGE_UPLOADER_H_

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

class ImageUploader {
 public:
  ImageUploader();

  /**
   * @brief 画像をサーバーにアップロードする
   * @return アップロード成功時true、失敗時false
   */
  bool static uploadImage(const std::string& imagePath, int maxAttempts = 3);
};

#endif  // IMAGE_UPLOADER_H_
