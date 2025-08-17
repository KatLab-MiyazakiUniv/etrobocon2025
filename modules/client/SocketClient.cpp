#include "SocketClient.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 27015

SocketClient::SocketClient() : sock(-1), isConnected(false) {}

SocketClient::~SocketClient()
{
  if(isConnected) {
    disconnectFromServer();
  }
}

bool SocketClient::connectToServer(const char* server_ip)
{
  if(isConnected) {
    std::cout << "Already connected." << std::endl;
    return true;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    perror("Client: socket creation failed");
    return false;
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if(inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
    perror("Client: Invalid address/ Address not supported");
    close(sock);
    sock = -1;
    return false;
  }

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Client: Connection Failed");
    close(sock);
    sock = -1;
    return false;
  }

  std::cout << "Successfully connected to camera server." << std::endl;
  isConnected = true;
  return true;
}

void SocketClient::disconnectFromServer()
{
  if(isConnected) {
    CameraServer::Command cmd = CameraServer::Command::SHUTDOWN;
    send(sock, reinterpret_cast<const char*>(&cmd), sizeof(cmd), 0);
    close(sock);
    sock = -1;
    isConnected = false;
    std::cout << "Disconnected from camera server." << std::endl;
  }
}

bool SocketClient::executeMiniFigAction(const CameraServer::MiniFigActionRequest& request,
                                        CameraServer::MiniFigActionResponse& response)
{
  if(!isConnected) {
    std::cerr << "Not connected to server." << std::endl;
    return false;
  }

  // 1. Send the request
  if(send(sock, reinterpret_cast<const char*>(&request), sizeof(request), 0) < 0) {
    perror("Client: send failed");
    return false;
  }

  // 2. Wait for and receive the response
  ssize_t bytesRead = recv(sock, reinterpret_cast<char*>(&response), sizeof(response), 0);
  if(bytesRead < 0) {
    perror("Client: recv failed");
    return false;
  } else if(bytesRead != sizeof(response)) {
    std::cerr << "Client: received incomplete response." << std::endl;
    return false;
  }

  return true;
}

bool SocketClient::executeBackgroundPlaAction(
    const CameraServer::BackgroundPlaActionRequest& request,
    CameraServer::BackgroundPlaActionResponse& response)
{
  if(!isConnected) {
    std::cerr << "Not connected to server." << std::endl;
    return false;
  }

  // 1. Send the request
  if(send(sock, reinterpret_cast<const char*>(&request), sizeof(request), 0) < 0) {
    perror("Client: send failed");
    return false;
  }

  // 2. Wait for and receive the response
  ssize_t bytesRead = recv(sock, reinterpret_cast<char*>(&response), sizeof(response), 0);
  if(bytesRead < 0) {
    perror("Client: recv failed");
    return false;
  } else if(bytesRead != sizeof(response)) {
    std::cerr << "Client: received incomplete response." << std::endl;
    return false;
  }

  return true;
}

bool SocketClient::executeSnapshotAction(const CameraServer::SnapshotActionRequest& request,
                                         CameraServer::SnapshotActionResponse& response)
{
  if(!isConnected) {
    std::cerr << "Not connected to server." << std::endl;
    return false;
  }

  // 1. Send the request
  if(send(sock, reinterpret_cast<const char*>(&request), sizeof(request), 0) < 0) {
    perror("Client: send failed");
    return false;
  }

  // 2. Wait for and receive the response
  ssize_t bytesRead = recv(sock, reinterpret_cast<char*>(&response), sizeof(response), 0);
  if(bytesRead < 0) {
    perror("Client: recv failed");
    return false;
  } else if(bytesRead != sizeof(response)) {
    std::cerr << "Client: received incomplete response." << std::endl;
    return false;
  }

  return true;
}

bool SocketClient::executeLineDetection(const CameraServer::BoundingBoxDetectorRequest& request,
                                        CameraServer::BoundingBoxDetectorResponse& response)
{
  if(!isConnected) {
    std::cerr << "Not connected to server." << std::endl;
    return false;
  }

  // 1. Send the request
  if(send(sock, reinterpret_cast<const char*>(&request), sizeof(request), 0) < 0) {
    perror("Client: send failed");
    return false;
  }

  // 2. Wait for and receive the response
  ssize_t bytesRead = recv(sock, reinterpret_cast<char*>(&response), sizeof(response), 0);
  if(bytesRead < 0) {
    perror("Client: recv failed");
    return false;
  } else if(bytesRead != sizeof(response)) {
    std::cerr << "Client: received incomplete response." << std::endl;
    return false;
  }

  return true;
}
