/**
 * @file   Socket.cpp
 * @brief  ネットワーク通信のためのソケット操作クラスの実装
 * @author takuchi17
 */

#include "Socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

Socket::Socket() : sock(-1) {}

Socket::Socket(int sock_fd) : sock(sock_fd) {}

Socket::~Socket()
{
  close();
}

bool Socket::create()
{
  sock = socket(AF_INET, SOCK_STREAM, 0);
  return sock != -1;
}

bool Socket::bind(int port)
{
  if(!isValid()) return false;

  sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port);

  return ::bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) != -1;
}

bool Socket::listen() const
{
  if(!isValid()) return false;
  return ::listen(sock, 5) != -1;
}

Socket* Socket::accept() const
{
  if(!isValid()) return nullptr;

  sockaddr_in client_address{};
  socklen_t len = sizeof(client_address);
  int client_sock = ::accept(sock, (struct sockaddr*)&client_address, &len);

  if(client_sock < 0) {
    return nullptr;
  }
  return new Socket(client_sock);
}

bool Socket::connect(const std::string& host, int port)
{
  if(!isValid()) {
    if(!create()) return false;
  }

  sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  if(inet_pton(AF_INET, host.c_str(), &server_address.sin_addr) <= 0) {
    return false;
  }

  return ::connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) != -1;
}

bool Socket::send(const std::string& message) const
{
  if(!isValid()) return false;
  return ::send(sock, message.c_str(), message.length(), 0) != -1;
}

int Socket::receive(std::string& message) const
{
  if(!isValid()) return -1;

  char buffer[1024] = { 0 };
  message = "";

  int bytes_received = ::recv(sock, buffer, sizeof(buffer) - 1, 0);
  if(bytes_received > 0) {
    message.assign(buffer, bytes_received);
  }
  return bytes_received;
}

// New method implementations
bool Socket::sendData(const void* data, size_t size) const
{
  if(!isValid() || data == nullptr) return false;

  const char* buffer = static_cast<const char*>(data);
  size_t total_sent = 0;
  while(total_sent < size) {
    ssize_t sent_now = ::send(sock, buffer + total_sent, size - total_sent, 0);
    if(sent_now == -1) {
      return false;  // Error
    }
    total_sent += sent_now;
  }
  return true;
}

bool Socket::receiveData(void* buffer, size_t size) const
{
  if(!isValid() || buffer == nullptr) return false;

  char* char_buffer = static_cast<char*>(buffer);
  size_t total_received = 0;
  while(total_received < size) {
    ssize_t received_now = ::recv(sock, char_buffer + total_received, size - total_received, 0);
    if(received_now == -1) {
      return false;  // Error
    }
    if(received_now == 0) {
      return false;  // Connection closed
    }
    total_received += received_now;
  }
  return true;
}

bool Socket::isValid() const
{
  return sock != -1;
}

void Socket::close()
{
  if(isValid()) {
    ::close(sock);
    sock = -1;
  }
}