#ifndef BACKGROUND_PLA_ACTION_HANDLER_H
#define BACKGROUND_PLA_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "BackgroundDirectionDetector.h"
#include "MotionDetector.h"
#include "SocketProtocol.h"

class BackgroundPlaActionHandler {
 public:
  BackgroundPlaActionHandler(CameraCapture& camera);

  void execute(int position, CameraServer::BackgroundPlaActionResponse& response);

 private:
  void runPlaCameraAction();

  CameraCapture& camera;
  BackgroundDirectionDetector directionDetector;
  BackgroundDirectionResult firstAttemptResult;
  const char* filePath = "etrobocon2025/datafiles/snapshots/";
};

#endif  // BACKGROUND_PLA_ACTION_HANDLER_H
