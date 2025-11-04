/**
 * @file TwoColorLineDetectionActionHandler.cpp
 * @brief 2色線検出をするクラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#include "TwoColorLineDetectionActionHandler.h"
#include <iostream>

TwoColorLineDetectionActionHandler::TwoColorLineDetectionActionHandler(CameraCapture& _camera)
  : camera(_camera)
{
}

void TwoColorLineDetectionActionHandler::execute(
    const CameraServer::TwoColorBoundingBoxDetectorRequest& request,
    CameraServer::BoundingBoxDetectorResponse& response)
{
  std::cout << "Executing TWO_COLOR_LINE_DETECTION command." << std::endl;

  cv::Mat frame;
  if(!camera.getFrame(frame)) {
    std::cerr << "Failed to capture frame for line detection." << std::endl;
    response.result.wasDetected = false;
    return;
  }

  // Create a TwoColorBoundingBoxDetector with parameters from the request
  TwoColorBoundingBoxDetector detector(request.lowerFirstHSV, request.upperFirstHSV,
                                       request.lowerSecondHSV, request.upperSecondHSV, request.roi,
                                       request.resolution);

  detector.detect(frame, response.result);

  if(response.result.wasDetected) {
    std::cout << "Line detected successfully." << std::endl;
  } else {
    std::cout << "Line not detected." << std::endl;
  }
}
