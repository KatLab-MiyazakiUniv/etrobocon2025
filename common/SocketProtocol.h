#ifndef SOCKET_PROTOCOL_H
#define SOCKET_PROTOCOL_H

#include "ImageRecognitionResults.h"

#include <cstdint>
#include <opencv2/core/types.hpp>  // For cv::Rect

namespace CameraServer {

  // Enum for all possible commands
  enum class Command : uint8_t {
    MINIFIG_CAMERA_ACTION = 0,
    BACKGROUND_PLA_CAMERA_ACTION = 1,
    SHUTDOWN = 255
  };

  // Data structure for the MiniFigCameraAction request/response
  struct MiniFigActionRequest {
    Command command;   // Should always be MINIFIG_CAMERA_ACTION
    int32_t position;  // 0 for the first attempt, 1-3 for subsequent ones
  };
  struct MiniFigActionResponse {
    MiniFigDirectionResult result;
  };

  // Data structure for the BackgroundPlaCameraAction request/response
  struct BackgroundPlaActionRequest {
    Command command;  // Should always be BACKGROUND_PLA_CAMERA_ACTION
    int32_t position;
    double threshold;
    double minArea;
    cv::Rect roi;
  };
  struct BackgroundPlaActionResponse {
    BackgroundDirectionResult result;
  };

  struct BoundingBoxDetectorRequest {
    cv::Scalar lowerHSV;
    cv::Scalar upperHSV;
    cv::Rect roi;
    cv::Size resolution;
  };
  struct BoundingBoxDetectorResponse {
    BoundingBoxDetectionResult result;
  };

}  // namespace CameraServer

#endif  // SOCKET_PROTOCOL_H
