/**
 * @file   DummyCameraCapture.h
 * @brief  カメラを制御するクラス（ダミー）
 * @author HaruArima08 miyahara046
 */

#ifndef DUMMY_CAMERA_CAPTURE_H
#define DUMMY_CAMERA_CAPTURE_H

#include "ICameraCapture.h"
#include <queue>

class DummyCameraCapture : public ICameraCapture {
 public:
  int findAvailableCameraID(int maxTested = 10) override { return 0; }
  int getCameraID() override { return 0; }
  bool setCameraID(int id) override { return true; }
  bool openCamera() override { return true; }
  void setCapProps(double width, double height) override {}
  /**
   * @brief フレーム取得の成功・失敗の順番を設定
   */
  void setFrameResults(const std::vector<bool>& results)
  {
    while(!frameResults.empty()) frameResults.pop();
    for(bool r : results) frameResults.push(r);
    hasCustomFlag = true;
  }
  /**
   * @brief 1枚のカメラ画像を取得する
   * @param outFrame 取得した画像を格納するcv::Mat参照
   * @return 画像取得に成功した場合はtrue、失敗した場合はfalse
   */
  bool getFrame(cv::Mat& outFrame) override
  {
    if(hasCustomFlag) {
      if(frameResults.empty()) {
        outFrame = cv::Mat();
        return false;
      }
      bool success = frameResults.front();
      frameResults.pop();
      if(success) {
        outFrame = cv::Mat::zeros(480, 640, CV_8UC3);  // 黒画像
        return true;
      } else {
        outFrame = cv::Mat();  // 空画像（失敗）
        return false;
      }
    } else {
      // デフォルトでは常に成功で黒画像を返す
      outFrame = cv::Mat::zeros(480, 640, CV_8UC3);
      return true;
    }
  }

  bool getFrames(std::vector<cv::Mat>& frames, int numFrames, int millisecondInterval) override
  {
    return true;
  }

 private:
  std::queue<bool> frameResults;  // フレーム取得の成功・失敗の順番を保持する
  bool hasCustomFlag = false;     // setFrameResults()で結果リストを設定したかどうかを表すフラグ
};

#endif