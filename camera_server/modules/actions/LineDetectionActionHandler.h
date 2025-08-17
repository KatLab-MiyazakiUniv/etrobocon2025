#ifndef LINE_DETECTION_ACTION_HANDLER_H
#define LINE_DETECTION_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "LineBoundingBoxDetector.h"
#include "SocketProtocol.h"

class LineDetectionActionHandler {
 public:
  LineDetectionActionHandler(CameraCapture& camera);

  void execute(const CameraServer::BoundingBoxDetectorRequest& request,
               CameraServer::BoundingBoxDetectorResponse& response);

 private:
  CameraCapture& camera;
};

#endif  // LINE_DETECTION_ACTION_HANDLER_H
