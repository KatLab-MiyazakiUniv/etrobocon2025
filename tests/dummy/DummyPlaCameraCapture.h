/**
 * @file   DummyCameraCapture.h
 * @brief  カメラを制御するクラス（ダミー）
 * @author HaruArima08 miyahara046
 */

#ifndef DUMMY_PLA_CAMERA_CAPTURE_H
#define DUMMY_PLA_CAMERA_CAPTURE_H

#include "ICameraCapture.h"
#include <queue>
#include <vector>

class DummyPlaCameraCapture : public ICameraCapture {
 public:
  DummyPlaCameraCapture() = default;

  // ------ ICameraCapture の基本関数の実装 ------
  int findAvailableCameraID(int maxTested = 10) override { return 0; }
  int getCameraID() override { return 0; }
  bool setCameraID(int id) override { return true; }
  bool openCamera() override { return true; }
  void setCapProps(double width, double height) override {}

  // ------ 1枚ずつ取得 ------
  bool getFrame(cv::Mat& outFrame) override
  {
    if(currentIndex < frames.size()) {
      outFrame = frames[currentIndex++];
      return true;
    } else {
      outFrame = cv::Mat();  // 取得失敗（空画像）
      return false;
    }
  }

  // ------ 複数フレームを取得 ------
  bool getFrames(std::vector<cv::Mat>& outFrames, int numFrames, int millisecondInterval) override
  {
    outFrames.clear();
    for(int i = 0; i < numFrames; ++i) {
      if(currentIndex < frames.size()) {
        outFrames.push_back(frames[currentIndex++]);
      } else {
        break;
      }
    }
    return outFrames.size() == numFrames;
  }

  // ------ テスト用：ダミーフレームの挿入 ------
  void setDummyFrames(int count, cv::Scalar color = cv::Scalar(0, 0, 0))
  {
    frames.clear();
    currentIndex = 0;
    for(int i = 0; i < count; ++i) {
      frames.push_back(cv::Mat(480, 640, CV_8UC3, color));
    }
  }
  void setMotionLikeFrames()
  {
    frames.clear();
    currentIndex = 0;

    // [0～6] 静止状態（真っ黒）
    for(int i = 0; i < 7; ++i) {
      frames.push_back(cv::Mat(480, 640, CV_8UC3, cv::Scalar(0, 0, 0)));
    }

    // [7～11] 動きがある状態（白い矩形が少しずつ右へ動く）
    for(int i = 0; i < 5; ++i) {
      cv::Mat frame = cv::Mat(480, 640, CV_8UC3, cv::Scalar(0, 0, 0));
      int x = 100 + i * 10;
      cv::rectangle(frame, cv::Point(x, 100), cv::Point(x + 50, 150), cv::Scalar(255, 255, 255),
                    -1);
      frames.push_back(frame);
    }

    // [12～14] 再び静止（真っ黒）
    for(int i = 0; i < 3; ++i) {
      frames.push_back(cv::Mat(480, 640, CV_8UC3, cv::Scalar(0, 0, 0)));
    }
  }

 private:
  std::vector<cv::Mat> frames;
  size_t currentIndex = 0;
};

#endif  // PLA_DUMMY_CAMERA_CAPTURE_H
