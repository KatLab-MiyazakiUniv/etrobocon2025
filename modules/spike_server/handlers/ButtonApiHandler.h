/**
 * @file   ButtonApiHandler.h
 * @brief  ボタンAPIを処理するハンドラクラス
 * @author takuchi17
 */

#ifndef BUTTON_API_HANDLER_H
#define BUTTON_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "Button.h"

class ButtonApiHandler : public ApiHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  explicit ButtonApiHandler(Socket* client);

  /**
   * @brief ボタンの押下状態を処理する
   * @param request ボタン押下リクエスト
   */
  void handleIsPressed(const spike::ButtonIsPressedRequest& request);

 private:
  spikeapi::Button button;  // ボタンインスタンス
};

#endif  // BUTTON_API_HANDLER_H
