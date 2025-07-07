/**
 * @file   FrameSave.cpp
 * @brief  写真を保存するクラス
 * @author nishijima515
 */
#include "FrameSave.h"

// const std::string FrameSave::filepath = "etrobocon2025/datafiles/検討中/";

// FrameSave::FrameSave(Robot& _robot, cv::Mat& _frame, const std::string& _filename)
//   : robot(_robot), frame(_frame), filename(_filename)
// {
//   // 保存先のディレクトリを設定
//   filepath = "etrobocon2025/datafiles/検討中/";
// }

void FrameSave::frameSave(cv::Mat& frame, const std::string& filepath, const std::string& filename)
{
  // 画像を保存
  CameraCapture cameraCapture;
  if(!cameraCapture.saveFrame(frame, filepath, filename)) {
    printf("画像の保存に失敗しました: %s\n", (filepath + filename).c_str());
  } else {
    printf("画像を保存しました: %s\n", (filepath + filename).c_str());
  }
}
