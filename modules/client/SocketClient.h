#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include "SocketProtocol.h"

class SocketClient {
 public:
  SocketClient();
  ~SocketClient();

  bool connectToServer(const char* server_ip = "127.0.0.1");
  void disconnectFromServer();

  bool executeMiniFigAction(const CameraServer::MiniFigActionRequest& request,
                            CameraServer::MiniFigActionResponse& response);
  bool executeBackgroundPlaAction(const CameraServer::BackgroundPlaActionRequest& request,
                                  CameraServer::BackgroundPlaActionResponse& response);

  bool executeSnapshotAction(const CameraServer::SnapshotActionRequest& request,
                             CameraServer::SnapshotActionResponse& response);

  bool executeLineDetection(const CameraServer::BoundingBoxDetectorRequest& request,
                            CameraServer::BoundingBoxDetectorResponse& response);

 private:
  int sock;
  bool isConnected;
};

#endif  // SOCKET_CLIENT_H
