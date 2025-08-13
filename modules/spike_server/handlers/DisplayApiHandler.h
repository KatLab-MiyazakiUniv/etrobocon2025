/**
 * @file   DisplayApiHandler.h
 * @brief  ディスプレイAPIを処理するハンドラクラス
 * @author takuchi17
 */

#ifndef DISPLAY_API_HANDLER_H
#define DISPLAY_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "Display.h"

class DisplayApiHandler : public ApiHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  explicit DisplayApiHandler(Socket* client);

  /**
   * @brief 文字表示を処理する
   * @param request 文字表示リクエスト
   */
  void handleShowChar(const spike::DisplayShowCharRequest& request);
  /**
   * @brief 数値表示を処理する
   * @param request 数値表示リクエスト
   */
  void handleShowNumber(const spike::DisplayShowNumberRequest& request);
  /**
   * @brief テキストスクロール表示を処理する
   * @param request テキストスクロールリクエスト
   */
  void handleScrollText(const spike::DisplayScrollTextRequest& request);

 private:
  spikeapi::Display display;  // ディスプレイインスタンス
};

#endif  // DISPLAY_API_HANDLER_H
