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

  SocketClient& client = robot.getSocketClient();

  CameraServer::SnapshotActionRequest request;
  request.command = CameraServer::Command::TAKE_SNAPSHOT;

  strncpy(request.fileName, fileName.c_str(), sizeof(request.fileName) - 1);
  request.fileName[sizeof(request.fileName) - 1] = '\0';

  CameraServer::SnapshotActionResponse response;

  bool success = client.executeSnapshotAction(request, response);
  if(success) {
    std::cout << "Snapshot taken successfully." << std::endl;
  } else {
    std::cerr << "Server failed to take snapshot." << std::endl;
  }
}