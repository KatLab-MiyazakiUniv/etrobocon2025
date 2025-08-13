/**
 * @file   ApiHandler.h
 * @brief  APIハンドラの基底クラス
 * @author takuchi17
 */

#ifndef API_HANDLER_H
#define API_HANDLER_H

#include "Socket.h"
#include "SpikeCommand.h"
#include <iostream>

class ApiHandler {
 protected:
  Socket* client_;  // クライアントソケットへのポインタ

  /**
   * @brief データを受信するヘルパー関数
   * @param buffer 受信バッファ
   * @param size 受信サイズ
   * @return 成功した場合true、失敗した場合false
   */
  bool receive(char* buffer, size_t size);

  /**
   * @brief データを送信するヘルパー関数
   * @param buffer 送信バッファ
   * @param size 送信サイズ
   * @return 成功した場合true、失敗した場合false
   */
  bool send(const char* buffer, size_t size);

 public:
  /**
   * @brief コンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  explicit ApiHandler(Socket* client);
  /**
   * @brief デストラクタ
   */
  virtual ~ApiHandler() = default;
};

#endif  // API_HANDLER_H
