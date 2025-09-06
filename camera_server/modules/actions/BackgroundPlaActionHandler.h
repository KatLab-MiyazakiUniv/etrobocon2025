/**
 * @file   BackgroundPlaActionHandler.h
 * @brief  背景・プラレール撮影をするクラス
 * @author miyahara046 takuchi17
 */

#ifndef BACKGROUND_PLA_ACTION_HANDLER_H
#define BACKGROUND_PLA_ACTION_HANDLER_H

#include "ImageRecognitionResults.h"

#include "CameraCapture.h"
#include "BackgroundDirectionDetector.h"
#include "MotionDetector.h"
#include "SocketProtocol.h"

class BackgroundPlaActionHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param camera カメラキャプチャのインスタンス
   */
  BackgroundPlaActionHandler(CameraCapture& camera);

  /**
   * @brief 背景・プラレール撮影アクションを実行する
   * @param request クライアントからのリクエスト
   * @param response クライアントへのレスポンス
   */
  void execute(const CameraServer::BackgroundPlaActionRequest& request,
               CameraServer::BackgroundPlaActionResponse& response);

 private:
  /**
   * @brief プラレール撮影シーケンスを実行する
   * @param request クライアントからのリクエスト
   */
  void runPlaCameraAction(const CameraServer::BackgroundPlaActionRequest& request);

  CameraCapture& camera;
  BackgroundDirectionDetector directionDetector;
  BackgroundDirectionResult firstAttemptResult;
  int shot_count = 0;  // 撮影回数カウンター
  const char* filePath = "datafiles/snapshots/";
};

#endif  // BACKGROUND_PLA_ACTION_HANDLER_H
