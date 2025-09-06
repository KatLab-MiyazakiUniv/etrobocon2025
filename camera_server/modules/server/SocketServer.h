/**
 * @file SocketServer.h
 * @brief 接続を待ち、クライアントからのリクエストを処理するクラス
 * @author takuchi17
 */

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "MiniFigActionHandler.h"
#include "BackgroundPlaActionHandler.h"
#include "SnapshotActionHandler.h"
#include "LineDetectionActionHandler.h"
#include <vector>

class SocketServer {
 public:
  /**
   * @brief コンストラクタ
   * @param minifigHandler ミニフィグ撮影アクションハンドラ
   * @param bgPlaHandler 背景・プラレール撮影アクションハンドラ
   * @param snapshotHandler スナップショット撮影アクションハンドラ
   * @param lineDetectionHandler ライン検出アクションハンドラ
   */
  SocketServer(MiniFigActionHandler& minifigHandler, BackgroundPlaActionHandler& bgPlaHandler,
               SnapshotActionHandler& snapshotHandler,
               LineDetectionActionHandler& lineDetectionHandler);

  /**
   * @brief サーバーを初期化する
   * @return true 初期化に成功した場合
   * @return false 初期化に失敗した場合
   */
  bool init();

  /**
   * @brief サーバーを実行し、クライアントからの接続を待機する
   */
  void run();

  /**
   * @brief サーバーをシャットダウンする
   */
  void shutdown();

 private:
  int listenSocket;
  bool isRunning;
  MiniFigActionHandler& minifigHandler;
  BackgroundPlaActionHandler& bgPlaHandler;
  SnapshotActionHandler& snapshotHandler;
  LineDetectionActionHandler& lineDetectionHandler;

  /**
   * @brief クライアントとの接続を処理する
   * @param clientSocket クライアントソケット
   */
  void handle_connection(int clientSocket);
};

#endif  // SOCKET_SERVER_H
