/**
 * @file   SpikeClient.h
 * @brief  SPIKEサーバーと通信するためのクライアントクラス
 * @author takuchi17
 */

#ifndef SPIKE_CLIENT_H
#define SPIKE_CLIENT_H

#include "Socket.h"
#include "SpikeCommand.h"
#include <string>
#include <vector>
#include <optional>
#include <mutex>  // For std::mutex and std::lock_guard

class SpikeClient {
 public:
  /**
   * @brief コンストラクタ
   */
  SpikeClient();
  /**
   * @brief サーバーに接続する
   * @param host ホスト名またはIPアドレス
   * @param port ポート番号
   * @return 接続成功の場合true、失敗の場合false
   */
  bool connect(const std::string& host, int port);
  /**
   * @brief サーバーから切断する
   */
  void disconnect();
  /**
   * @brief コマンドを実行する
   * @param id コマンドID
   * @return 実行成功の場合true、失敗の場合false
   */
  inline bool executeCommand(spike::CommandId id)
  {
    std::lock_guard<std::mutex> lock(mtx);  // Acquire lock
    if(!socket.isValid()) return false;
    spike::Request header{ id };
    if(!socket.sendData(&header, sizeof(header))) return false;
    spike::Response response;
    if(!socket.receiveData(&response, sizeof(response))) return false;
    return response.value;
  }

  /**
   * @brief リクエストを伴うコマンドを実行する
   * @tparam TRequest リクエストの型
   * @param id コマンドID
   * @param request リクエストデータ
   * @return 実行成功の場合true、失敗の場合false
   */
  template <typename TRequest>
  bool executeCommand(spike::CommandId id, const TRequest& request)
  {
    std::lock_guard<std::mutex> lock(mtx);  // Acquire lock
    if(!socket.isValid()) return false;
    spike::Request header{ id };
    if(!socket.sendData(&header, sizeof(header))) return false;
    if(!socket.sendData(&request, sizeof(request))) return false;
    spike::Response response;
    if(!socket.receiveData(&response, sizeof(response))) return false;
    return response.value;
  }

  /**
   * @brief リクエストとレスポンスを伴うクエリを実行する
   * @tparam TResponse レスポンスの型
   * @tparam TRequest リクエストの型
   * @param id コマンドID
   * @param request リクエストデータ
   * @return レスポンスデータ（成功の場合）、またはstd::nullopt（失敗の場合）
   */
  template <typename TResponse, typename TRequest>
  std::optional<TResponse> executeQuery(spike::CommandId id, const TRequest& request)
  {
    std::lock_guard<std::mutex> lock(mtx);  // Acquire lock
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

  /**
   * @brief レスポンスを伴うクエリを実行する
   * @tparam TResponse レスポンスの型
   * @param id コマンドID
   * @return レスポンスデータ（成功の場合）、またはstd::nullopt（失敗の場合）
   */
  template <typename TResponse>
  std::optional<TResponse> executeQuery(spike::CommandId id)
  {
    std::lock_guard<std::mutex> lock(mtx);  // Acquire lock
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
  Socket socket;   // ソケットインスタンス
  std::mutex mtx;  // Mutex for thread-safe socket access
};

#endif
