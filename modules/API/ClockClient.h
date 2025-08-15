/**
 * @file   ClockClient.h
 * @brief  時刻操作のためのクライアントAPI
 * @author takuchi17
 */

#ifndef CLOCK_CLIENT_H
#define CLOCK_CLIENT_H

#include <cstdint>
#include "APIClient.h"  // Inherit from APIClient
#include "SpikeCommand.h"

class ClockClient : public APIClient {
 public:
  /**
   * @brief コンストラクタ
   * @param client SpikeClientの参照
   */
  explicit ClockClient(SpikeClient& client);
  /**
   * @brief 指定したミリ秒間スリープする
   * @param milliseconds スリープ時間（ミリ秒）
   */
  void sleep(uint64_t microseconds);
  /**
   * @brief 現在時刻を取得する
   * @return 現在時刻（ミリ秒）
   */
  uint64_t now();

 private:
  // SpikeClient& spikeClient; // Moved to base class APIClient
};

#endif
