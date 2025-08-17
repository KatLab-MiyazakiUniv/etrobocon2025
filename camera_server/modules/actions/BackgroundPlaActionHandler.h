#ifndef BACKGROUND_PLA_ACTION_HANDLER_H
#define BACKGROUND_PLA_ACTION_HANDLER_H

#include "ImageRecognitionResults.h"

#include "CameraCapture.h"
#include "BackgroundDirectionDetector.h"
#include "MotionDetector.h"
#include "SocketProtocol.h"

class BackgroundPlaActionHandler {
 public:
  BackgroundPlaActionHandler(CameraCapture& camera);

  void execute(const CameraServer::BackgroundPlaActionRequest& request,
             CameraServer::BackgroundPlaActionResponse& response);

 private:
  void runPlaCameraAction(const CameraServer::BackgroundPlaActionRequest& request);

  CameraCapture& camera;
  BackgroundDirectionDetector directionDetector;
  BackgroundDirectionResult firstAttemptResult;
  const char* filePath = "etrobocon2025/datafiles/snapshots/";
};

#endif  // BACKGROUND_PLA_ACTION_HANDLER_H
