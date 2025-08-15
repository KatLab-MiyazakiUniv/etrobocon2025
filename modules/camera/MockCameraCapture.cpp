/**
 * @file   MockCameraCapture.h
 * @brief  カメラキャプチャーのモッククラス
 * @author takuchi17
 */

#include "MockCameraCapture.h"
#include <opencv2/opencv.hpp>
#include <iostream>

MockCameraCapture::MockCameraCapture() : currentCameraID(-1)
{
  // Initialize a dummy frame (e.g., a black image)
  dummyFrame = cv::Mat::zeros(800, 600, CV_8UC3);  // 640x480, 3 channels, black
  std::cout << "MockCameraCapture: Initialized." << std::endl;
}

int MockCameraCapture::findAvailableCameraID(int maxTested)
{
  std::cout << "MockCameraCapture: findAvailableCameraID called. Returning 0." << std::endl;
  return 0;  // Always return 0 for mock
}

int MockCameraCapture::getCameraID()
{
  std::cout << "MockCameraCapture: getCameraID called. Returning " << currentCameraID << "."
            << std::endl;
  return currentCameraID;
}

bool MockCameraCapture::setCameraID(int id)
{
  std::cout << "MockCameraCapture: setCameraID called. Setting to " << id << "." << std::endl;
  currentCameraID = id;
  return true;
}

bool MockCameraCapture::openCamera()
{
  std::cout << "MockCameraCapture: openCamera called. Returning true." << std::endl;
  return true;
}

void MockCameraCapture::setCapProps(double width, double height)
{
  std::cout << "MockCameraCapture: setCapProps called. Width: " << width << ", Height: " << height
            << "." << std::endl;
  // In a mock, we don't actually set properties on a real camera
}

bool MockCameraCapture::getFrame(cv::Mat& outFrame)
{
  std::cout << "MockCameraCapture: getFrame called. Returning dummy frame." << std::endl;
  dummyFrame.copyTo(outFrame);
  return true;
}

bool MockCameraCapture::getFrames(std::vector<cv::Mat>& frames, int numFrames,
                                  int millisecondInterval)
{
  std::cout << "MockCameraCapture: getFrames called. Returning " << numFrames << " dummy frames."
            << std::endl;
  frames.clear();
  for(int i = 0; i < numFrames; ++i) {
    cv::Mat frame_copy;
    dummyFrame.copyTo(frame_copy);
    frames.push_back(frame_copy);
  }
  return true;
}
