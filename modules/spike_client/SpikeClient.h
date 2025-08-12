#ifndef SPIKE_CLIENT_H
#define SPIKE_CLIENT_H

#include "Socket.h"
#include "SpikeCommand.h"
#include <string>
#include <vector>
#include <optional>

class SpikeClient {
 public:
  SpikeClient();
  bool connect(const std::string& host, int port);
  void disconnect();
  inline bool executeCommand(spike::CommandId id)
  {
    if(!socket.isValid()) return false;
    spike::Request header{ id };
    if(!socket.sendData(&header, sizeof(header))) return false;
    spike::Response response;
    if(!socket.receiveData(&response, sizeof(response))) return false;
    return response.value;
  }

  template <typename TRequest>
  bool executeCommand(spike::CommandId id, const TRequest& request)
  {
    if(!socket.isValid()) return false;
    spike::Request header{ id };
    if(!socket.sendData(&header, sizeof(header))) return false;
    if(!socket.sendData(&request, sizeof(request))) return false;
    spike::Response response;
    if(!socket.receiveData(&response, sizeof(response))) return false;
    return response.value;
  }

  template <typename TResponse, typename TRequest>
  std::optional<TResponse> executeQuery(spike::CommandId id, const TRequest& request)
  {
    if(!socket.isValid()) return std::nullopt;
    spike::Request header{ id };
    if(!socket.sendData(&header, sizeof(header))) return std::nullopt;
    if(!socket.sendData(&request, sizeof(request))) return std::nullopt;
    spike::Response response_header;
    if(!socket.receiveData(&response_header, sizeof(response_header))) return std::nullopt;
    if(!response_header.value) return std::nullopt;
    TResponse response_payload;
    if(!socket.receiveData(&response_payload, sizeof(response_payload))) return std::nullopt;
    return response_payload;
  }

  template <typename TResponse>
  std::optional<TResponse> executeQuery(spike::CommandId id)
  {
    if(!socket.isValid()) return std::nullopt;
    spike::Request header{ id };
    if(!socket.sendData(&header, sizeof(header))) return std::nullopt;
    spike::Response response_header;
    if(!socket.receiveData(&response_header, sizeof(response_header))) return std::nullopt;
    if(!response_header.value) return std::nullopt;
    TResponse response_payload;
    if(!socket.receiveData(&response_payload, sizeof(response_payload))) return std::nullopt;
    return response_payload;
  }

 private:
  Socket socket;
};

#endif
