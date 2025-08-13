/**
 * @file   ForceSensorClient.h
 * @brief  フォースセンサー操作のためのクライアントAPI
 * @author takuchi17
 */

#ifndef FORCESENSOR_CLIENT_H
#define FORCESENSOR_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"

class ForceSensorClient {
 public:
  /**
   * @brief コンストラクタ
   * @param client SpikeClientの参照
   */
  explicit ForceSensorClient(SpikeClient& client);
  /**
   * @brief 押されているか
   * @param threshold 閾値
   * @return 押されている場合true、それ以外false
   */
  bool isPressed(float threshold = 0.5f);
  /**
   * @brief フォース値を取得する
   * @return フォース値
   */
  float getForce();

 private:
  SpikeClient& spikeClient;  // SpikeClientの参照
};

#endif
