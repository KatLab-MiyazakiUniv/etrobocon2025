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
  if(frame.empty()) {
    std::cerr << "保存するフレームがありません。" << std::endl;
  }

  // ディレクトリが存在しない場合は作成
  if(!std::filesystem::exists(filepath)) {
    if(!std::filesystem::create_directories(filepath)) {
      std::cerr << "ディレクトリの作成に失敗しました: " << filepath << std::endl;
    }
  }

  std::string imagePath = filepath + "/" + filename + imagePath;
  if(!cv::imwrite(imagePath, frame)) {
    std::cerr << "画像の保存に失敗しました: " << imagePath << std::endl;
  }
}
