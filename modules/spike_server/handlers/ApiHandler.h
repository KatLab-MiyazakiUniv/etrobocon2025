#ifndef API_HANDLER_H
#define API_HANDLER_H

#include "Socket.h"
#include "SpikeCommand.h"
#include <iostream>

class ApiHandler {
 protected:
  Socket* client_;

  // Helper to receive data
  bool receive(char* buffer, size_t size)
  {
    bool success = client_->receiveData(buffer, size);
    if(!success) {
      std::cerr << "Error: Expected " << size << " bytes, but received " << std::endl;
    }
    return success;
  }

  // Helper to send data
  bool send(const char* buffer, size_t size)
  {
    bool success = client_->sendData(buffer, size);
    if(!success) {
      std::cerr << "Error: Expected to send " << size << " bytes, but sent " << std::endl;
    }
    return success;
  }

 public:
  explicit ApiHandler(Socket* client) : client_(client) {}
  virtual ~ApiHandler() = default;
};

#endif  // API_HANDLER_H
