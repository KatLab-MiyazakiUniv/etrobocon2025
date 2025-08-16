#include "server/SocketServer.h"
#include <iostream>
#include <string.h> // For memset
#include <unistd.h> // For close
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 27015
#define DEFAULT_BUFLEN 512

SocketServer::SocketServer(MiniFigActionHandler& _minifigHandler, BackgroundPlaActionHandler& _bgPlaHandler)
  : listenSocket(-1), isRunning(false), minifigHandler(_minifigHandler), bgPlaHandler(_bgPlaHandler) {}

bool SocketServer::init() {
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        perror("socket failed");
        return false;
    }

    int opt = 1;
    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(listenSocket);
        return false;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(listenSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        close(listenSocket);
        return false;
    }

    if (listen(listenSocket, 3) < 0) {
        perror("listen failed");
        close(listenSocket);
        return false;
    }

    std::cout << "Socket server initialized and listening on port " << PORT << std::endl;
    return true;
}

void SocketServer::run() {
    isRunning = true;
    while (isRunning) {
        int clientSocket = accept(listenSocket, (struct sockaddr*)NULL, NULL);
        if (clientSocket < 0) {
            if (!isRunning) break; // Shutdown was called
            perror("accept failed");
            continue;
        }
        std::cout << "Client connected." << std::endl;
        handle_connection(clientSocket);
        close(clientSocket);
        std::cout << "Client disconnected." << std::endl;
    }
}

void SocketServer::shutdown() {
    isRunning = false;
    if (listenSocket != -1) {
        close(listenSocket);
        listenSocket = -1;
    }
    std::cout << "Socket server shutting down." << std::endl;
}

void SocketServer::handle_connection(int clientSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    ssize_t iResult;

    do {
        iResult = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            if (static_cast<size_t>(iResult) >= sizeof(CameraServer::Command)) {
                CameraServer::Command cmd = *reinterpret_cast<CameraServer::Command*>(recvbuf);

                if (cmd == CameraServer::Command::MINIFIG_CAMERA_ACTION) {
                    if (static_cast<size_t>(iResult) == sizeof(CameraServer::MiniFigActionRequest)) {
                        auto* request = reinterpret_cast<CameraServer::MiniFigActionRequest*>(recvbuf);
                        std::cout << "Executing MINIFIG_CAMERA_ACTION for position " << request->position << std::endl;
                        
                        CameraServer::MiniFigActionResponse response;
                        minifigHandler.execute(request->position, response);
                        
                        send(clientSocket, reinterpret_cast<const char*>(&response), sizeof(response), 0);
                    } else {
                        std::cerr << "Invalid request size for MINIFIG_CAMERA_ACTION." << std::endl;
                    }
                } else if (cmd == CameraServer::Command::BACKGROUND_PLA_CAMERA_ACTION) {
                    if (static_cast<size_t>(iResult) == sizeof(CameraServer::BackgroundPlaActionRequest)) {
                        auto* request = reinterpret_cast<CameraServer::BackgroundPlaActionRequest*>(recvbuf);
                        std::cout << "Executing BACKGROUND_PLA_CAMERA_ACTION for position " << request->position << std::endl;
                        
                        CameraServer::BackgroundPlaActionResponse response;
                        bgPlaHandler.execute(*request, response);
                        
                        send(clientSocket, reinterpret_cast<const char*>(&response), sizeof(response), 0);
                    } else {
                        std::cerr << "Invalid request size for BACKGROUND_PLA_CAMERA_ACTION." << std::endl;
                    }
                } else if (cmd == CameraServer::Command::SHUTDOWN) {
                    std::cout << "Received SHUTDOWN command." << std::endl;
                    shutdown();
                    break;
                } else {
                    std::cerr << "Received unknown command." << std::endl;
                }
            } else {
                 std::cerr << "Received unexpected data size: " << iResult << std::endl;
            }
        } else if (iResult == 0) {
            std::cout << "Connection closing..." << std::endl;
        } else {
            if (isRunning) perror("recv failed");
        }
    } while (iResult > 0);
}
