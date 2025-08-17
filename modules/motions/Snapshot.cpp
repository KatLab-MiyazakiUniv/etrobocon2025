/**
 * @file   Snapshot.cpp
 * @brief  サーバーに写真撮影を依頼するクラス
 * @author takuchi17
 */

#include "Snapshot.h"
#include "SocketClient.h"
#include <iostream>
#include <string.h>

Snapshot::Snapshot(Robot& _robot, const std::string& _fileName)
  : Motion(_robot), fileName(_fileName)
{
}

void Snapshot::run()
{
  std::cout << "Requesting snapshot: " << fileName << std::endl;

  // Get SocketClient from Robot
  SocketClient& client = robot.getSocketClient();

  // Create request
  CameraServer::SnapshotActionRequest request;
  request.command = CameraServer::Command::TAKE_SNAPSHOT;

  // Safely copy fileName to the fixed-size char array
  strncpy(request.fileName, fileName.c_str(), sizeof(request.fileName) - 1);
  request.fileName[sizeof(request.fileName) - 1] = '\0';  // Ensure null-termination

  // Create response
  CameraServer::SnapshotActionResponse response;

  // Execute the action
  if(client.executeSnapshotAction(request, response)) {
    if(response.success) {
      std::cout << "Snapshot taken successfully." << std::endl;
    } else {
      std::cerr << "Server failed to take snapshot." << std::endl;
    }
  } else {
    std::cerr << "Failed to send snapshot request to server." << std::endl;
  }
}