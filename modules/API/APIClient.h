/**
 * @file   APIClient.h
 * @brief  サーバーと通信を行うクライアントクラスの抽象クラス
 * @author takuchi17
 */

#ifndef API_CLIENT_H
#define API_CLIENT_H

#include "SpikeClient.h"

class APIClient {
 public:
  /**
   * @brief コンストラクタ
   * @param client SpikeClientの参照
   */
  explicit APIClient(SpikeClient& client);

  virtual ~APIClient() = 0;

 protected:
  SpikeClient& spikeClient;  // SpikeClientの参照
};

#endif  // API_CLIENT_H