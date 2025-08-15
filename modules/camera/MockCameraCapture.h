/**
 * @file   MockCameraCapture.h
 * @brief  カメラキャプチャーのモッククラス
 * @author takuchi17
 */

#ifndef MOCK_CAMERA_CAPTURE_H
#define MOCK_CAMERA_CAPTURE_H

#include "ICameraCapture.h"
#include <iostream>

class MockCameraCapture : public ICameraCapture {
 public:
  MockCameraCapture();
  ~MockCameraCapture() override = default;

  int findAvailableCameraID(int maxTested = 10) override;
  int getCameraID() override;
  bool setCameraID(int id) override;
  bool openCamera() override;
  void setCapProps(double width, double height) override;
  bool getFrame(cv::Mat& outFrame) override;
  bool getFrames(std::vector<cv::Mat>& frames, int numFrames, int millisecondInterval) override;

 private:
  int currentCameraID;
  cv::Mat dummyFrame;
};

#endif  // MOCK_CAMERA_CAPTURE_H
