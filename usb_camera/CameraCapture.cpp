/**
 * @file   AreaACameraAction.cpp
 * @brief  カメラを制御するクラス
 * @author Haranaruki
 */

#include "CameraCapture.h"

CameraCapture::CameraCapture() : cameraID(0) {}

int CameraCapture::getAvailableCameraID(int maxTested)
{
  for(int i = 0; i < maxTested; ++i) {
    cv::VideoCapture cap(i);
    if(cap.isOpened()) {
      cap.release();
      return i;
    }
  }
  return -1;
}

int CameraCapture::getCameraID()
{
  return cameraID;
}

bool CameraCapture::setCameraID(int id)
{
  if(id < 0) {
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
    std::cerr << "カメラを開いて開いてください。" << std::endl;
    return false;
  }
  cap >> outFrame;
  if(outFrame.empty()) {
    std::cerr << "フレームの取得に失敗しました。" << std::endl;
    return false;
  }
  return true;
}

bool CameraCapture::getFrames(cv::Mat* frames, int numFrames, double seconds)
{
  if(frames == nullptr || numFrames <= 0 || seconds <= 0) return false;
  int intervalMs = static_cast<int>((seconds * 1000) / numFrames);
  bool allSuccess = true;
  for(int i = 0; i < numFrames; ++i) {
    if(!getFrame(frames[i])) {
      std::cerr << "フレーム " << i << " の取得に失敗しました。" << std::endl;
      allSuccess = false;
    }
    if(i < numFrames - 1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
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