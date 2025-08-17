#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "MiniFigActionHandler.h"
#include "BackgroundPlaActionHandler.h"
#include "SnapshotActionHandler.h"
#include <vector>

class SocketServer {
 public:
  SocketServer(MiniFigActionHandler& minifigHandler, BackgroundPlaActionHandler& bgPlaHandler,
               SnapshotActionHandler& snapshotHandler);
  bool init();
  void run();
  void shutdown();

 private:
  int listenSocket;
  bool isRunning;
  MiniFigActionHandler& minifigHandler;
  BackgroundPlaActionHandler& bgPlaHandler;
  SnapshotActionHandler& snapshotHandler;

  void handle_connection(int clientSocket);
};

#endif  // SOCKET_SERVER_H
