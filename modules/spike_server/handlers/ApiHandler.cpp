#include "ApiHandler.h"
#include <iostream>  // Include iostream for std::cerr

ApiHandler::ApiHandler(Socket* client) : client_(client) {}

bool ApiHandler::receive(char* buffer, size_t size)
{
  bool success = client_->receiveData(buffer, size);
  if(!success) {
    std::cerr << "Error: Expected " << size << " bytes, but received " << std::endl;
  }
  return success;
}

bool ApiHandler::send(const char* buffer, size_t size)
{
  bool success = client_->sendData(buffer, size);
  if(!success) {
    std::cerr << "Error: Expected to send " << size << " bytes, but sent " << std::endl;
  }
  return success;
}
