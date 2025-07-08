/**
 * @file   FrameSave.cpp
 * @brief  写真を保存するクラス
 * @author nishijima515
 */
#include "FrameSave.h"

void FrameSave::frameSave(cv::Mat& frame, const std::string& filePath, const std::string& fileName)
{
  if(frame.empty()) {
    std::cerr << "保存するフレームがありません。" << std::endl;
  }

  // ディレクトリが存在しない場合は作成
  if(!std::filesystem::exists(filePath)) {
    if(!std::filesystem::create_directories(filePath)) {
      std::cerr << "ディレクトリの作成に失敗しました: " << filePath << std::endl;
    }
  }

  std::string imagePath = filePath + "/" + fileName + imgExtension;
  if(!cv::imwrite(imagePath, frame)) {
    std::cerr << "画像の保存に失敗しました: " << imagePath << std::endl;
  }
}
