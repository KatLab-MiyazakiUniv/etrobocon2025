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
  /**
   * @brief 画像をサーバーにアップロードする
   * @param  imagePath　送信する画像のパス
   * @param　maxAttempts　送信試行回数の上限
   * @return アップロード成功時true、失敗時false
   */
  bool static uploadImage(const std::string& imagePath, int maxAttempts = 3);

 private:
  ImageUploader();
};

#endif  // IMAGE_UPLOADER_H_
