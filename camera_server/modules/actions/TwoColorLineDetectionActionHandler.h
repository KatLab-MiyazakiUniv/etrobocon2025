/**
 * @file TwoColorLineDetectionActionHandler.h
 * @brief 2色線検出をするクラス
 * @author takuchi17 miyahara046 HaruArima08
 */

#ifndef TWO_COLOR_LINE_DETECTION_ACTION_HANDLER_H
#define TWO_COLOR_LINE_DETECTION_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "TwoColorLineBoundingBoxDetector.h"
#include "SocketProtocol.h"

class TwoColorLineDetectionActionHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param camera カメラキャプチャのインスタンス
   */
  TwoColorLineDetectionActionHandler(CameraCapture& camera);

  /**
   * @brief ライン検出アクションを実行する
   * @param request クライアントからのリクエスト
   * @param response クライアントへのレスポンス
   */
  void execute(const CameraServer::TwoColorBoundingBoxDetectorRequest& request,
               CameraServer::BoundingBoxDetectorResponse& response);

 private:
  CameraCapture& camera;
};

#endif  // TWO_COLOR_LINE_DETECTION_ACTION_HANDLER_H
