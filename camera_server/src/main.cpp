#include <iostream>
#include "camera/CameraCapture.h"
#include "actions/MiniFigActionHandler.h"
#include "actions/BackgroundPlaActionHandler.h"
#include "server/SocketServer.h"

int main() {
    std::cout << "Camera server starting..." << std::endl;

    // 1. Initialize Camera
    CameraCapture camera;
    int cameraId = camera.findAvailableCameraID();
    if (cameraId < 0) {
        std::cerr << "No available camera found." << std::endl;
        return 1;
    }
    camera.setCameraID(cameraId);
    if (!camera.openCamera()) {
        std::cerr << "Failed to open camera." << std::endl;
        return 1;
    }
    std::cout << "Camera initialized successfully." << std::endl;

    // 2. Create Action Handlers
    MiniFigActionHandler minifigHandler(camera);
    BackgroundPlaActionHandler bgPlaHandler(camera);

    // 3. Initialize and run the server
    SocketServer server(minifigHandler, bgPlaHandler);
    if (!server.init()) {
        std::cerr << "Failed to initialize socket server." << std::endl;
        return 1;
    }

    server.run();

    std::cout << "Camera server has shut down." << std::endl;
    return 0;
}
