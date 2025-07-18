/**
 * @file   FrameSave.h
 * @brief  写真を保存するクラス
 * @author nishijima515
 */

#ifndef FRAME_SAVE_H
#define FRAME_SAVE_H

#include "Robot.h"
#include "CameraCapture.h"
#include <opencv2/opencv.hpp>
#include <filesystem>  // std::filesystemを使用するために追加
#include <string>

class FrameSave {
 public:
  /**
   * @brief 画像を保存する
   * @param frame 保存する画像を格納するcv::Mat参照
   * @param filePath　画像保存先のディレクトリパス
   * @param fileName 保存する画像の名前
   */
  static void save(cv::Mat& frame, const std::string& filePath, const std::string& fileName);

 private:
  inline static std::string imgExtension = ".JPEG";  // 保存するときの拡張子
  FrameSave();                                       // インスタンス化の禁止
};

#endif  // FRAME_SAVE_H