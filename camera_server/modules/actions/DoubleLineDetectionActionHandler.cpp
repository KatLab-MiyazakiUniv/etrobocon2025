/**
 * @file DoubleLineDetectionActionHandler.cpp
 * @brief 2色線検出をするクラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#include "DoubleLineDetectionActionHandler.h"
#include <iostream>

DoubleLineDetectionActionHandler::DoubleLineDetectionActionHandler(CameraCapture& _camera)
  : camera(_camera)
{
}

void DoubleLineDetectionActionHandler::execute(
    const CameraServer::BoundingBoxDetectorRequest& request,
    CameraServer::BoundingBoxDetectorResponse& response)
{
  std::cout << "Executing DOUBLE_LINE_DETECTION command." << std::endl;

  cv::Mat frame;
  if(!camera.getFrame(frame)) {
    std::cerr << "Failed to capture frame for line detection." << std::endl;
    response.result.wasDetected = false;
    return;
  }

  // Create a DoubleLineBoundingBoxDetector with parameters from the request
  DoubleLineBoundingBoxDetector detector(request.lowerFirstHSV, request.upperFirstHSV,
                                         request.lowerSecondHSV, request.upperSecondHSV,
                                         request.roi, request.resolution);

  detector.detect(frame, response.result);

  if(response.result.wasDetected) {
    std::cout << "Line detected successfully." << std::endl;
  } else {
    std::cout << "Line not detected." << std::endl;
  }
}
