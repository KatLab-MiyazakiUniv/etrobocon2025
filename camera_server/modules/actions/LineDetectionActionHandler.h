/**
 * @file LineDetectionActionHandler.h
 * @brief 線検出をするクラス
 * @author takuchi17
 */

#ifndef LINE_DETECTION_ACTION_HANDLER_H
#define LINE_DETECTION_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "LineBoundingBoxDetector.h"
#include "SocketProtocol.h"

class LineDetectionActionHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param camera カメラキャプチャのインスタンス
   */
  LineDetectionActionHandler(CameraCapture& camera);

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
