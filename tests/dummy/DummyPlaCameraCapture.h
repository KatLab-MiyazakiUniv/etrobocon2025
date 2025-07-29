/**
 * @file   DummyPlaCameraCapture.h
 * @brief  カメラを制御するクラス（PlaCameraAction用ダミー）
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
  int findAvailableCameraID(int maxTested = 10) override { return 0; }
  int getCameraID() override { return 0; }
  bool setCameraID(int id) override { return true; }
  bool openCamera() override { return true; }
  void setCapProps(double width, double height) override {}

  /**
   * @brief 1枚のカメラ画像を取得する
   * @param outFrame 取得した画像を格納するcv::Mat参照
   * @return 画像取得に成功した場合はtrue、失敗した場合はfalse
   */
  bool getFrame(cv::Mat& outFrame) override
  {
    if(currentIndex < frames.size()) {
      outFrame = frames[currentIndex++];
      return true;
    } else {
      // フレームが枯渇した場合は最後のフレーム（静止状態）を繰り返し返す
      if(!frames.empty()) {
        outFrame = frames.back();
        return true;
      } else {
        outFrame = cv::Mat();  // 取得失敗（空画像）
        return false;
      }
    }
  }

  /**
   * @brief 複数のカメラ画像を取得する
   * @param outFrame 取得した画像を格納するcv::Mat参照
   * @param numFrames 取得したフレームの総数
   * @param millisecondInterval 取得するフレームのインターバル
   * @return 取得したフレームの総数
   */
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

  /**
   * @brief 単色の静止フレームを複数枚挿入する（初期化用）
   * @param count 挿入するフレーム枚数
   * @param color 単色の色（デフォルトは黒）
   */
  void setDummyFrames(int count, cv::Scalar color = cv::Scalar(0, 0, 0))
  {
    frames.clear();
    currentIndex = 0;
    for(int i = 0; i < count; ++i) {
      frames.push_back(cv::Mat(480, 640, CV_8UC3, color));
    }
  }

  /**
   * @brief 動体のあるフレーム列を挿入する（動体検出テスト用）
   * 最初と最後は静止状態の黒画面
   * 中央で白い矩形が少しずつ右へ動く（動きのあるフレーム）
   */
  void setMotionLikeFrames()
  {
    frames.clear();
    currentIndex = 0;

    // [0～15] 静止状態（真っ黒）- getBackgroundFrame()が確実に背景を見つけられるよう増加
    for(int i = 0; i < 16; ++i) {
      frames.push_back(cv::Mat(480, 640, CV_8UC3, cv::Scalar(0, 0, 0)));
    }

    // [16～20] 動きがある状態（白い矩形が少しずつ右へ動く）
    for(int i = 0; i < 5; ++i) {
      cv::Mat frame = cv::Mat(480, 640, CV_8UC3, cv::Scalar(0, 0, 0));
      int x = 100 + i * 10;
      cv::rectangle(frame, cv::Point(x, 100), cv::Point(x + 50, 150), cv::Scalar(255, 255, 255),
                    -1);
      frames.push_back(frame);
    }

    // [21～30] 再び静止（真っ黒）- 退出検出用
    for(int i = 0; i < 10; ++i) {
      frames.push_back(cv::Mat(480, 640, CV_8UC3, cv::Scalar(0, 0, 0)));
    }
  }

 private:
  std::vector<cv::Mat> frames;
  size_t currentIndex = 0;
};

#endif  // DUMMY_PLA_CAMERA_CAPTURE_H
