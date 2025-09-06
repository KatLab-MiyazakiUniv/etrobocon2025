/**
 * @file BackgroundPlaActionHandler.cpp
 * @brief 風景・プラレール撮影をするクラス
 * @author miyahara046 takuchi17
 */

#include "BackgroundPlaActionHandler.h"
#include "FrameSave.h"
#include "ImageUploader.h"
#include <thread>
#include <iostream>

using namespace std;

constexpr int MAX_NO_MOTION = 5;  // 動きなしと判定する連続フレーム数

BackgroundPlaActionHandler::BackgroundPlaActionHandler(CameraCapture& _camera) : camera(_camera)
{
  firstAttemptResult.wasDetected = false;
}

void BackgroundPlaActionHandler::runPlaCameraAction(
    const CameraServer::BackgroundPlaActionRequest& request)
{
  cout << "Starting PlaCameraAction sequence..." << endl;
  // Directly use cv::Rect from the request
  MotionDetector motionDetector(request.threshold, request.minArea, request.roi);

  // Get background frame
  cv::Mat firstFrame;
  while(true) {
    for(int i = 0; i < 5; i++) {
      camera.getFrame(firstFrame);
      this_thread::sleep_for(chrono::milliseconds(33));
    }
    cv::Mat secondFrame;
    this_thread::sleep_for(chrono::milliseconds(50));
    camera.getFrame(secondFrame);
    if(motionDetector.compareTwoFrames(firstFrame, secondFrame).empty()) {
      break;  // No motion detected, we have a good background
    }
  }
  motionDetector.setBackground(firstFrame);
  cout << "Background frame captured." << endl;

  // Detect motion start and end
  vector<cv::Mat> capturedFrames;
  BoundingBoxDetectionResult detectionResult;
  while(true) {  // Wait for motion to start
    cv::Mat frame;
    camera.getFrame(frame);
    this_thread::sleep_for(chrono::milliseconds(33));
    motionDetector.detect(frame, detectionResult);
    if(detectionResult.wasDetected) {
      capturedFrames.push_back(frame.clone());
      break;
    }
  }
  cout << "Motion started." << endl;

  int noMotionCounter = 0;
  while(true) {  // Wait for motion to end
    cv::Mat frame;
    camera.getFrame(frame);
    this_thread::sleep_for(chrono::milliseconds(33));
    motionDetector.detect(frame, detectionResult);
    if(detectionResult.wasDetected) {
      noMotionCounter = 0;
    } else {
      noMotionCounter++;
      if(noMotionCounter >= MAX_NO_MOTION) {
        break;
      }
    }
    capturedFrames.push_back(frame.clone());
  }
  cout << "Motion ended." << endl;

  // Save the best frame and upload
  if(!capturedFrames.empty()) {
    cv::Mat bestFrame = capturedFrames.at(capturedFrames.size() / 2);
    string fileName = "PlaRail";
    FrameSave::save(bestFrame, filePath, fileName);
    thread([path = string(filePath), name = fileName] {
      ImageUploader::uploadImage(path, name, 3);
    }).detach();
    cout << "Saved and initiated upload for " << fileName << endl;
  }
}

void BackgroundPlaActionHandler::execute(const CameraServer::BackgroundPlaActionRequest& request,
                                         CameraServer::BackgroundPlaActionResponse& response)
{
  cv::Mat frame;
  for(int i = 0; i < 5; i++) {  // Capture a few frames to stabilize
    camera.getFrame(frame);
    this_thread::sleep_for(chrono::milliseconds(33));
  }

  if(frame.empty()) {
    cerr << "Failed to capture frame." << endl;
    response.result.wasDetected = false;
    return;
  }

  if(shot_count == 0) {
    cout << "First attempt (shot_count 0). Detecting background direction." << endl;
    directionDetector.detect(frame, this->firstAttemptResult);

    if(this->firstAttemptResult.wasDetected
       && this->firstAttemptResult.direction == BackgroundDirection::FRONT) {
      cout << "Direction is FRONT. Running PlaCameraAction." << endl;
      runPlaCameraAction(request);
    } else if(!this->firstAttemptResult.wasDetected) {
      cout << "Direction not detected. Running PlaCameraAction." << endl;
      runPlaCameraAction(request);
    }
    response.result.wasDetected = this->firstAttemptResult.wasDetected;
    response.result.direction = this->firstAttemptResult.direction;
  } else if(this->firstAttemptResult.wasDetected) {
    cout << "Subsequent attempt (shot_count " << shot_count << "). Assuming FRONT." << endl;
    runPlaCameraAction(request);
  }
  shot_count++;
}