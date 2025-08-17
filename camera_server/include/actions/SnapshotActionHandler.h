#ifndef SNAPSHOT_ACTION_HANDLER_H
#define SNAPSHOT_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "SocketProtocol.h"

class SnapshotActionHandler {
 public:
  SnapshotActionHandler(CameraCapture& camera);

  void execute(const CameraServer::SnapshotActionRequest& request,
               CameraServer::SnapshotActionResponse& response);

 private:
  CameraCapture& camera;
  const char* filePath = "etrobocon2025/datafiles/snapshots/";
};

#endif  // SNAPSHOT_ACTION_HANDLER_H
