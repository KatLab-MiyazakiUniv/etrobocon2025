/**
 * @file   AreaACameraAction.cpp
 * @brief  カメラを制御するクラス
 * @author Hara1274
 */

#include "CameraCapture.h"

CameraCapture::CameraCapture()
  : cap(),       // カメラデバイス（未接続で初期化）
    cameraID(0)  // カメラIDを0で初期化
{
}

int CameraCapture::findAvailableCameraID(int maxTested)
{
  for(int i = 0; i < maxTested; ++i) {
    cv::VideoCapture cap(i);
    if(cap.isOpened()) {
      cap.release();
      // 利用可能なIDを返す
      if(cap.isOpened()) {
        cap.release();
        // 利用可能なIDを返す
        std::cerr << "カメラID:" << i << " を使ってください" << std::endl;
        return i;
      }
      std::cerr << "使用できるカメラIDが存在しません" << std::endl;
      return i;
    }
  }
  // どのIDも利用できなかった場合は-1を返す
  return -1;
}

int CameraCapture::getCurrentCameraID()
{
  return cameraID;
}

bool CameraCapture::setCameraID(int id)
{
  if(id < 0) {
    std::cerr << "無効なカメラIDです" << std::endl;
    return false;
  }
  cameraID = id;
  return true;
}

bool CameraCapture::openCamera()
{
  cap.open(cameraID);
  if(!cap.isOpened()) {
    std::cerr << "カメラを開くことができませんでした。" << std::endl;
    return false;
  }

  return true;
}

void CameraCapture::setCapProps(double width, double height)
{
  cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
}

bool CameraCapture::getFrame(cv::Mat& outFrame)
{
  if(!cap.isOpened()) {
    std::cerr << "カメラを開くことができませんでした。" << std::endl;
    return false;
  }
  cap >> outFrame;
  if(outFrame.empty()) {
    std::cerr << "フレームの取得に失敗しました。" << std::endl;
    return false;
  }
  return true;
}

bool CameraCapture::getFrames(cv::Mat* frames, int numFrames, int millisecondInterval)
{
  if(frames == nullptr || numFrames <= 0 || millisecondInterval <= 0) return false;
  bool allSuccess = true;
  for(int i = 0; i < numFrames; ++i) {
    if(!getFrame(frames[i])) {
      std::cerr << "フレーム " << i << " の取得に失敗しました。" << std::endl;
      allSuccess = false;
    }
    if(i < numFrames - 1) {
      // 最後の1回以外は、intervals ミリ秒だけ待機
      std::this_thread::sleep_for(std::chrono::milliseconds(millisecondInterval));
    }
  }
  return allSuccess;
}

bool CameraCapture::saveFrame(const cv::Mat& frame, std::string filepath, std::string filename)
{
  if(frame.empty()) {
    std::cerr << "保存するフレームがありません。" << std::endl;
    return false;
  }
  std::string fullpath = filepath + "/" + filename + ".JPEG";
  if(!cv::imwrite(fullpath, frame)) {
    std::cerr << "画像の保存に失敗しました: " << fullpath << std::endl;
    return false;
  }
  return true;
}