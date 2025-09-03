#include "MiniFigActionHandler.h"
#include "FrameSave.h"
#include "ImageUploader.h"
#include <thread>
#include <iostream>

using namespace std;

MiniFigActionHandler::MiniFigActionHandler(CameraCapture& _camera) : camera(_camera)
{
  firstAttemptResult.wasDetected = false;  // Initialize state
}

void MiniFigActionHandler::detectDirection(cv::Mat& frame, MiniFigDirectionResult& result)
{
  cout << "Detecting direction..." << endl;
  detector.detect(frame, result);
  cout << "Detection finished." << endl;

  // Debug output
  if(!result.wasDetected) {
    cout << "Minifig not detected." << endl;
    return;
  }
  switch(result.direction) {
    case MiniFigDirection::FRONT:
      cout << "Direction: FRONT" << endl;
      break;
    case MiniFigDirection::BACK:
      cout << "Direction: BACK" << endl;
      break;
    case MiniFigDirection::LEFT:
      cout << "Direction: LEFT" << endl;
      break;
    case MiniFigDirection::RIGHT:
      cout << "Direction: RIGHT" << endl;
      break;
  }
}

void MiniFigActionHandler::execute(int position, CameraServer::MiniFigActionResponse& response)
{
  // 1. Take a picture
  cv::Mat frame;
  for(int i = 0; i < 5; i++) {  // Capture a few frames to stabilize
    camera.getFrame(frame);
  }

  if(frame.empty()) {
    cerr << "Failed to capture frame." << endl;
    response.result.wasDetected = false;
    return;
  }

  // 2. Execute logic based on position
  if(position == 0) {
    // This is the first attempt. Detect and store the result.
    cout << "First attempt (position 0)." << endl;
    detectDirection(frame, this->firstAttemptResult);

    if(this->firstAttemptResult.wasDetected
       && this->firstAttemptResult.direction == MiniFigDirection::FRONT) {
      cout << "Detected FRONT on first attempt. Saving image for upload." << endl;
      FrameSave::save(frame, filePath, uploadFileName);
      thread([path = string(filePath), name = string(uploadFileName)] {
        ImageUploader::uploadImage(path, name, 3);
      }).detach();
    }
    // The response for the first call is the result of that detection.
    response.result.wasDetected = this->firstAttemptResult.wasDetected;
    response.result.direction = this->firstAttemptResult.direction;

  } else if(this->firstAttemptResult.wasDetected) {
    // This is a subsequent attempt, and the first one was successful (but not FRONT).
    // This attempt must be the FRONT-facing one.
    cout << "Subsequent attempt (position " << position << "). Assuming FRONT." << endl;
    FrameSave::save(frame, filePath, uploadFileName);
    thread([path = string(filePath), name = string(uploadFileName)] {
      ImageUploader::uploadImage(path, name, 3);
    }).detach();
    response.result.wasDetected = true;
    response.result.direction = MiniFigDirection::FRONT;

  } else {
    // This is a subsequent attempt, but the first one failed to detect anything.
    cout << "Subsequent attempt (position " << position
         << ") after failed detection. Saving for debug." << endl;
    string positionImageName = "Fig_" + to_string(position) + ".jpg";
    FrameSave::save(frame, filePath, positionImageName);

    // Upload only on the last attempt if the first one failed.
    if(position == 3) {
      thread([path = string(filePath), name = positionImageName] {
        ImageUploader::uploadImage(path, name, 3);
      }).detach();
    }
    response.result.wasDetected = false;  // Still not considered a successful "FRONT" detection
  }
}
