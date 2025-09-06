/**
 * @file SnapshotActionHandler.cpp
 * @brief スナップショット撮影をするクラス
 * @author takuchi17
 */

#include "SnapshotActionHandler.h"
#include "FrameSave.h"
#include <iostream>

SnapshotActionHandler::SnapshotActionHandler(CameraCapture& _camera) : camera(_camera) {}

void SnapshotActionHandler::execute(const CameraServer::SnapshotActionRequest& request,
                                    CameraServer::SnapshotActionResponse& response)
{
  std::cout << "Executing TAKE_SNAPSHOT command for file: " << request.fileName << std::endl;

  cv::Mat frame;
  if(!camera.getFrame(frame)) {
    std::cerr << "Failed to capture frame for snapshot." << std::endl;
    response.success = false;
    return;
  }

  // The request.fileName is a char array, so it's already a C-style string.
  FrameSave::save(frame, filePath, request.fileName);

  std::cout << "Snapshot saved successfully." << std::endl;
  response.success = true;
}
