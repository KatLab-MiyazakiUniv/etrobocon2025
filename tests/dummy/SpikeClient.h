#ifndef DUMMY_SPIKE_CLIENT_H
#define DUMMY_SPIKE_CLIENT_H

#include "modules/common/SpikeCommand.h"
#include <string>
#include <vector>
#include <optional>
#include <map>
#include <functional>
#include <cstring>  // For memcpy

// Define a type for the response data and a map for handlers
struct MockResponse {
  bool success;
  std::vector<char> payload;
};

// Using std::function for more flexible handlers
using CommandHandler = std::function<MockResponse(const std::vector<char>& request_payload)>;

class SpikeClient {
 public:
  SpikeClient();
  bool connect(const std::string& host, int port);
  void disconnect();

  // Method to set up expected responses for commands
  void setCommandHandler(spike::CommandId id, CommandHandler handler);

  inline bool executeCommand(spike::CommandId id)
  {
    return executeCommand(id, std::vector<char>{});  // No request payload
  }

  template <typename TRequest>
  bool executeCommand(spike::CommandId id, const TRequest& request)
  {
    std::vector<char> request_payload(sizeof(TRequest));
    std::memcpy(request_payload.data(), &request, sizeof(TRequest));
    return handleCommand(id, request_payload).success;
  }

  template <typename TResponse, typename TRequest>
  std::optional<TResponse> executeQuery(spike::CommandId id, const TRequest& request)
  {
    std::vector<char> request_payload(sizeof(TRequest));
    std::memcpy(request_payload.data(), &request, sizeof(TRequest));
    MockResponse mock_res = handleCommand(id, request_payload);
    if(!mock_res.success || mock_res.payload.empty()) return std::nullopt;
    TResponse response_payload;
    std::memcpy(&response_payload, mock_res.payload.data(),
                std::min(mock_res.payload.size(), sizeof(TResponse)));
    return response_payload;
  }

  template <typename TResponse>
  std::optional<TResponse> executeQuery(spike::CommandId id)
  {
    MockResponse mock_res = handleCommand(id, std::vector<char>{});  // No request payload
    if(!mock_res.success || mock_res.payload.empty()) return std::nullopt;
    TResponse response_payload;
    std::memcpy(&response_payload, mock_res.payload.data(),
                std::min(mock_res.payload.size(), sizeof(TResponse)));
    return response_payload;
  }

 private:
  std::map<spike::CommandId, CommandHandler> commandHandlers_;

  // Internal handler that dispatches to registered handlers
  MockResponse handleCommand(spike::CommandId id, const std::vector<char>& request_payload);
};

#endif  // DUMMY_SPIKE_CLIENT_H
