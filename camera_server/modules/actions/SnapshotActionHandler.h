/**
 * @file SnapshotActionHandler.h
 * @brief スナップショット撮影をするクラス
 * @author takuchi17
 */

#ifndef SNAPSHOT_ACTION_HANDLER_H
#define SNAPSHOT_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "SocketProtocol.h"

class SnapshotActionHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param camera カメラキャプチャのインスタンス
   */
  SnapshotActionHandler(CameraCapture& camera);

  /**
   * @brief スナップショット撮影アクションを実行する
   * @param request クライアントからのリクエスト
   * @param response クライアントへのレスポンス
   */
  void execute(const CameraServer::SnapshotActionRequest& request,
               CameraServer::SnapshotActionResponse& response);

 private:
  CameraCapture& camera;
  const char* filePath = "datafiles/snapshots/";
};

#endif  // SNAPSHOT_ACTION_HANDLER_H
