/**
 * @file   MockSocketClient.h
 * @brief  SocketClientのモッククラス
 * @author takuchi17
 */

#ifndef MOCK_SOCKET_CLIENT_H
#define MOCK_SOCKET_CLIENT_H

#include "SocketClient.h"
#include <queue>
#include <optional>

class MockSocketClient : public SocketClient {
 public:
  MockSocketClient()
  {
    // テストでは、デフォルトで接続済みとして扱う
    isConnected = true;
  }
  ~MockSocketClient() override = default;

  // 基底クラスのメソッドをオーバーライド
  bool connectToServer(const char* server_ip) override
  {
    isConnected = true;
    return true;
  }

  void disconnectFromServer() override { isConnected = false; }

  bool executeMiniFigAction(const CameraServer::MiniFigActionRequest& request,
                            CameraServer::MiniFigActionResponse& response) override
  {
    if(miniFigResponse.has_value()) {
      response = miniFigResponse.value();
      return true;
    }
    return false;  // レスポンスが設定されていない
  }

  bool executeBackgroundPlaAction(const CameraServer::BackgroundPlaActionRequest& request,
                                  CameraServer::BackgroundPlaActionResponse& response) override
  {
    if(backgroundPlaResponse.has_value()) {
      response = backgroundPlaResponse.value();
      return true;
    }
    return false;
  }

  bool executeSnapshotAction(const CameraServer::SnapshotActionRequest& request,
                             CameraServer::SnapshotActionResponse& response) override
  {
    if(snapshotResponse.has_value()) {
      response = snapshotResponse.value();
      return true;
    }
    return false;
  }

  bool executeLineDetection(const CameraServer::BoundingBoxDetectorRequest& request,
                            CameraServer::BoundingBoxDetectorResponse& response) override
  {
    if(!lineDetectionResponses.empty()) {
      response = lineDetectionResponses.front();
      lineDetectionResponses.pop();
      return true;
    }
    // If the queue is empty, provide a default successful response for continuous line detection
    response.result.wasDetected = true;
    response.result.topLeft = cv::Point(270, 190);
    response.result.topRight = cv::Point(370, 190);
    response.result.bottomLeft = cv::Point(270, 290);
    response.result.bottomRight = cv::Point(370, 290);
    return true;
  }

  // --- テスト制御用のメソッド ---
  void setNextMiniFigResponse(const CameraServer::MiniFigActionResponse& resp)
  {
    miniFigResponse = resp;
  }

  void setNextBackgroundPlaResponse(const CameraServer::BackgroundPlaActionResponse& resp)
  {
    backgroundPlaResponse = resp;
  }

  void setNextSnapshotResponse(const CameraServer::SnapshotActionResponse& resp)
  {
    snapshotResponse = resp;
  }

  void setNextLineDetectionResponse(const CameraServer::BoundingBoxDetectorResponse& resp)
  {
    lineDetectionResponses.push(resp);
  }

 private:
  std::optional<CameraServer::MiniFigActionResponse> miniFigResponse;
  std::optional<CameraServer::BackgroundPlaActionResponse> backgroundPlaResponse;
  std::optional<CameraServer::SnapshotActionResponse> snapshotResponse;
  std::queue<CameraServer::BoundingBoxDetectorResponse> lineDetectionResponses;
};

#endif  // MOCK_SOCKET_CLIENT_H