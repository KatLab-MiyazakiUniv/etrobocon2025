/**
 * @file   CameraCapture.cpp
 * @brief  カメラを制御するクラス
 * @author Hara1274
 */

#include "CameraCapture.h"
using namespace std;

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
      cerr << "カメラID:" << i << " が見つかりました" << endl;
      return i;
    }
  }
  // どのIDも利用できなかった場合は-1を返す
  cerr << "使用できるカメラIDが見つかりません" << endl;
  return -1;
}

int CameraCapture::getCameraID()
{
  return cameraID;
}

bool CameraCapture::setCameraID(int id)
{
  if(id < 0) {
    cerr << "無効なカメラIDです" << endl;
    return false;
  }
  cameraID = id;
  return true;
}

bool CameraCapture::openCamera()
{
  cap.open(cameraID);
  if(!cap.isOpened()) {
    cerr << "カメラを開くことができませんでした。" << endl;
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
    cerr << "カメラを開いていません" << endl;
    return false;
  }
  cap >> outFrame;
  if(outFrame.empty()) {
    cerr << "フレームの取得に失敗しました。" << endl;
    return false;
  }
  return true;
}

bool CameraCapture::getFrames(cv::Mat& frames, int numFrames, int millisecondInterval)
{
  if(frames.empty()) {
    cerr << "フレームバッファが空です。" << endl;
    return false;
  }

  if(numFrames <= 0) {
    cerr << "フレーム数が無効です: " << numFrames << endl;
    return false;
  }
  if(millisecondInterval <= 0) {
    cerr << "インターバルが無効です: " << millisecondInterval << "ms" << endl;
    return false;
  }

  frames.resize(numFrames);
  bool allSuccess = true;
  for(int i = 0; i < numFrames; ++i) {
    if(!getFrame(frames[i])) {
      cerr << "フレーム " << i << " の取得に失敗しました。" << endl;
      allSuccess = false;
    }
    if(i < numFrames - 1) {
      // 最後の1回以外は、intervals ミリ秒だけ待機
      this_thread::sleep_for(chrono::milliseconds(millisecondInterval));
    }
  }
  return allSuccess;
}

bool CameraCapture::saveFrame(const cv::Mat& frame, string filepath, string filename)
{
  if(frame.empty()) {
    cerr << "保存するフレームがありません。" << endl;
    return false;
  }
  string imagePath = filepath + "/" + filename + imgExtension;
  if(!cv::imwrite(imagePath, frame)) {
    cerr << "画像の保存に失敗しました: " << imagePath << endl;
    return false;
  }
  return true;
}