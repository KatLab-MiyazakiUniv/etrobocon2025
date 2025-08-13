/**
 * @file   ClockApiHandler.h
 * @brief  時刻関連APIを処理するハンドラクラス
 * @author takuchi17
 */

#ifndef CLOCK_API_HANDLER_H
#define CLOCK_API_HANDLER_H

#include "ApiHandler.h"
#include "SpikeCommand.h"
#include "Clock.h"

class ClockApiHandler : public ApiHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  explicit ClockApiHandler(Socket* client);

  /**
   * @brief スリープ要求を処理する
   * @param request スリープ要求
   */
  void handleSleep(const spike::ClockSleepRequest& request);
  /**
   * @brief 現在時刻要求を処理する
   */
  void handleNow();

 private:
  spikeapi::Clock clock;  // クロックインスタンス
};

#endif  // CLOCK_API_HANDLER_H
