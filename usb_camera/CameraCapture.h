/**
 * @file   AreaACameraAction.cpp
 * @brief  カメラを制御するクラス
 * @author Haranaruki
 */

#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

class CameraCapture {
 private:
  cv::VideoCapture cap;
  int cameraID;
  cv::Mat latestFrame;

 public:
  CameraCapture();
  int getAvailableCameraID(int maxTested = 10);
  int getCameraID();
  bool setCameraID(int id);
  bool openCamera();
  void setCapProps(double width, double height);
  bool getFrame(cv::Mat& outFrame);
  bool getFrames(cv::Mat* frames, int numFrames, double seconds);
  bool saveFrame(const cv::Mat& frame, std::string filepath, std::string filename);
};

#endif