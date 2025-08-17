#include "LineDetectionActionHandler.h"
#include <iostream>

LineDetectionActionHandler::LineDetectionActionHandler(CameraCapture& _camera) : camera(_camera)
{
}

void LineDetectionActionHandler::execute(const CameraServer::BoundingBoxDetectorRequest& request,
                                         CameraServer::BoundingBoxDetectorResponse& response)
{
  std::cout << "Executing LINE_DETECTION command." << std::endl;

  cv::Mat frame;
  if (!camera.getFrame(frame)) {
    std::cerr << "Failed to capture frame for line detection." << std::endl;
    response.result.wasDetected = false;
    return;
  }

  // Create a LineBoundingBoxDetector with parameters from the request
  LineBoundingBoxDetector detector(request.lowerHSV, request.upperHSV, request.roi, request.resolution);
  
  detector.detect(frame, response.result);

  if (response.result.wasDetected) {
    std::cout << "Line detected successfully." << std::endl;
  } else {
    std::cout << "Line not detected." << std::endl;
  }
}
