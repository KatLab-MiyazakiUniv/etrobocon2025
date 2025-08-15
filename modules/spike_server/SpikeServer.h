/**
 * @file   SpikeServer.h
 * @brief  SPIKEデバイスとの通信を管理するサーバークラス
 * @author takuchi17
 */

#ifndef SPIKE_SERVER_H
#define SPIKE_SERVER_H

#include "spikeapi.h"
#include "SpikeCommand.h"
#include "MotorApiHandler.h"
#include "ColorSensorApiHandler.h"
#include "ClockApiHandler.h"
#include "ButtonApiHandler.h"
#include "ForceSensorApiHandler.h"
#include "DisplayApiHandler.h"
#include <string>
#include <vector>  // For potential future use with variable-length data

class Socket;  // Forward declaration

class SpikeServer {
 public:
  /**
   * @brief サーバーを開始する
   */
  static void start();

 private:
  /**
   * @brief APIハンドラを初期化するためのコンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  SpikeServer(Socket* client);

  /**
   * @brief 生データを受信する
   * @param client クライアントソケットへのポインタ
   * @param buffer 受信バッファ
   * @param size 受信サイズ
   * @return 成功した場合true、失敗した場合false
   */
  bool receive(Socket* client, char* buffer, size_t size);
  /**
   * @brief 生データを送信する
   * @param client クライアントソケットへのポインタ
   * @param buffer 送信バッファ
   * @param size 送信サイズ
   * @return 成功した場合true、失敗した場合false
   */
  bool send(Socket* client, const char* buffer, size_t size);

  /**
   * @brief コマンドを処理する
   * @param commandId コマンドID
   * @param client クライアントソケットへのポインタ
   */
  void handle_command(spike::CommandId commandId, Socket* client);

  // Member API handlers
  MotorApiHandler motorHandler;              // モーターAPIハンドラ
  ColorSensorApiHandler colorSensorHandler;  // カラーセンサーAPIハンドラ
  ClockApiHandler clockHandler;              // クロックAPIハンドラ
  ButtonApiHandler buttonHandler;            // ボタンAPIハンドラ
  ForceSensorApiHandler forceSensorHandler;  // フォースセンサーAPIハンドラ
  DisplayApiHandler displayHandler;          // ディスプレイAPIハンドラ
};

#endif  // SPIKE_SERVER_H