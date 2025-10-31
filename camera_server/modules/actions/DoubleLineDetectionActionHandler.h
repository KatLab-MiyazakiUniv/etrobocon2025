/**
 * @file DoubleLineDetectionActionHandler.h
 * @brief 2色線検出をするクラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#ifndef DOUBLE_LINE_DETECTION_ACTION_HANDLER_H
#define DOUBLE_LINE_DETECTION_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "DoubleLineBoundingBoxDetector.h"
#include "SocketProtocol.h"

class DoubleLineDetectionActionHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param camera カメラキャプチャのインスタンス
   */
  DoubleLineDetectionActionHandler(CameraCapture& camera);

  /**
   * @brief ライン検出アクションを実行する
   * @param request クライアントからのリクエスト
   * @param response クライアントへのレスポンス
   */
  void execute(const CameraServer::BoundingBoxDetectorRequest& request,
               CameraServer::BoundingBoxDetectorResponse& response);

 private:
  CameraCapture& camera;
};

#endif  // LINE_DETECTION_ACTION_HANDLER_H
