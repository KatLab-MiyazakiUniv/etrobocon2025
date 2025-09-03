/**
 * @file   CameraCapture.cpp
 * @brief  カメラを制御するクラス
 * @author Hara1274 takuchi17
 */

#include "CameraCapture.h"
#include <filesystem>

using namespace std;

CameraCapture::CameraCapture()
  : cap(),       // カメラデバイス（未接続で初期化）
    cameraID(0)  // カメラIDを0で初期化
{
}

CameraCapture::~CameraCapture()
{
  if(cap.isOpened()) {
    cap.release();
  }
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
  // V4L2を指定してカメラを開く
  cap.open(cameraID, cv::CAP_V4L2);
  if(!cap.isOpened()) {
    cerr << "カメラを開くことができませんでした。" << endl;
    return false;
  }

  // MJPEG形式に設定
  if(!cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'))) {
    cerr << "MJPEG形式の設定に失敗しました。" << endl;
    return false;
  }

  // FPSを30に設定
  if(!cap.set(cv::CAP_PROP_FPS, 30)) {
    cerr << "FPS設定に失敗しました。" << endl;
    return false;
  }

  // 解像度設定
  if(!cap.set(cv::CAP_PROP_FRAME_WIDTH, 800)) {
    cerr << "幅設定に失敗しました。" << endl;
    return false;
  }

  if(!cap.set(cv::CAP_PROP_FRAME_HEIGHT, 600)) {
    cerr << "高さ設定に失敗しました。" << endl;
    return false;
  }

  // 設定後の確認
  double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  double fps = cap.get(cv::CAP_PROP_FPS);
  cerr << "設定後の解像度: " << width << "x" << height << ", FPS: " << fps << endl;

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

  for(int i = 0; i < 5; i++) {
    cap >> outFrame;
    if(!outFrame.empty()) {
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  cerr << "フレームの取得に失敗しました。" << endl;
  return false;
}

bool CameraCapture::getFrames(vector<cv::Mat>& frames, int numFrames, int millisecondInterval)
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