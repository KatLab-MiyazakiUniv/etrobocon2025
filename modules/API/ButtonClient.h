/**
 * @file   ButtonClient.h
 * @brief  ボタン操作のためのクライアントAPI
 * @author takuchi17
 */

#ifndef BUTTON_CLIENT_H
#define BUTTON_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"

class ButtonClient {
 public:
  /**
   * @brief コンストラクタ
   * @param client SpikeClientの参照
   */
  explicit ButtonClient(SpikeClient& client);
  /**
   * @brief 右ボタンが押されているか
   * @return 押されている場合true、それ以外false
   */
  bool isRightPressed();
  /**
   * @brief 左ボタンが押されているか
   * @return 押されている場合true、それ以外false
   */
  bool isLeftPressed();
  /**
   * @brief 中央ボタンが押されているか
   * @return 押されている場合true、それ以外false
   */
  bool isCenterPressed();

 private:
  SpikeClient& spikeClient;  // SpikeClientの参照
};

#endif
