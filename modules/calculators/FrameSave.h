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
  // FrameSave(Robot& _robot, cv::Mat& _frame, const std::string& _filepath,
  //           const std::string& _filename);

  /**
   * @brief カメラ撮影を行い、画像を保存する
   */
  static void frameSave(cv::Mat& frame, const std::string& filepath, const std::string& filename);

 private:
  std::string filepath;  // 保存するパス
  std::string filename;  // 保存するファイル名
  cv::Mat& frame;        // カメラから取得した画像フレーム
  FrameSave();
};

#endif  // FRAME_SAVE_H