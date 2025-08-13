/**
 * @file   DisplayClient.h
 * @brief  ディスプレイ操作のためのクライアントAPI
 * @author takuchi17
 */

#ifndef DISPLAY_CLIENT_H
#define DISPLAY_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"
#include <string>

class DisplayClient {
 public:
  /**
   * @brief コンストラクタ
   * @param client SpikeClientの参照
   */
  explicit DisplayClient(SpikeClient& client);
  /**
   * @brief 文字を表示する
   * @param str 表示する文字
   */
  void showChar(const char str);
  /**
   * @brief 数値を表示する
   * @param number 表示する数値
   */
  void showNumber(const int8_t number);
  /**
   * @brief テキストをスクロール表示する
   * @param text 表示するテキスト
   * @param speed スクロール速度
   */
  void scrollText(const char* text, uint32_t delay = 50);

 private:
  SpikeClient& spikeClient;  // SpikeClientの参照
};

#endif
