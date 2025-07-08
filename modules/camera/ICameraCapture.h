/**
 * @file   ICameraCapture.h
 * @brief  カメラを制御するインターフェース
 * @author HaruArima08 miyahara046
 */

#ifndef I_CAMERA_CAPTURE_H
#define I_CAMERA_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class ICameraCapture {
 public:
  virtual ~ICameraCapture() = default;

  virtual int findAvailableCameraID(int maxTested = 10) = 0;
  virtual int getCameraID() = 0;
  virtual bool setCameraID(int id) = 0;
  virtual bool openCamera() = 0;
  virtual void setCapProps(double width, double height) = 0;
  virtual bool getFrame(cv::Mat& outFrame) = 0;
  virtual bool getFrames(std::vector<cv::Mat>& frames, int numFrames, int millisecondInterval) = 0;
};

#endif
