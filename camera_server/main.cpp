#include <iostream>
#include "CameraCapture.h"
#include "MiniFigActionHandler.h"
#include "BackgroundPlaActionHandler.h"
#include "SnapshotActionHandler.h"
#include "LineDetectionActionHandler.h"
#include "SocketServer.h"

int main()
{
  std::cout << "Camera server starting..." << std::endl;

  // カメラの初期化
  CameraCapture camera;
  int cameraId = camera.findAvailableCameraID();
  if(cameraId < 0) {
    std::cerr << "No available camera found." << std::endl;
    return 1;
  }
  camera.setCameraID(cameraId);
  if(!camera.openCamera()) {
    std::cerr << "Failed to open camera." << std::endl;
    return 1;
  }
  std::cout << "Camera initialized successfully." << std::endl;

  // ハンドラの初期化
  MiniFigActionHandler minifigHandler(camera);
  BackgroundPlaActionHandler bgPlaHandler(camera);
  SnapshotActionHandler snapshotHandler(camera);
  LineDetectionActionHandler lineDetectionHandler(camera);

  // サーバーの初期化と実行
  SocketServer server(minifigHandler, bgPlaHandler, snapshotHandler, lineDetectionHandler);
  if(!server.init()) {
    std::cerr << "Failed to initialize socket server." << std::endl;
    return 1;
  }

  server.run();

  std::cout << "Camera server has shut down." << std::endl;
  return 0;
}
