/**
 * @file   ForceSensorApiHandler.h
 * @brief  フォースセンサーAPIを処理するハンドラクラス
 * @author takuchi17
 */

#ifndef FORCE_SENSOR_API_HANDLER_H
#define FORCE_SENSOR_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "ForceSensor.h"

class ForceSensorApiHandler : public ApiHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  explicit ForceSensorApiHandler(Socket* client);

  /**
   * @brief 押下状態を処理する
   * @param request 押下状態リクエスト
   */
  void handleIsPressed(const spike::ForceSensorIsPressedRequest& request);
  /**
   * @brief フォース値取得を処理する
   */
  void handleGetForce();

 private:
  spikeapi::ForceSensor forceSensor;  // フォースセンサーインスタンス
};

#endif  // FORCE_SENSOR_API_HANDLER_H
