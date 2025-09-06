/**
 * @file MiniFigActionHandler.h
 * @brief ミニフィグ撮影をするクラス
 * @author nishijima515 takuchi17
 */

#ifndef MINIFIG_ACTION_HANDLER_H
#define MINIFIG_ACTION_HANDLER_H

#include "CameraCapture.h"
#include "MiniFigDirectionDetector.h"
#include "SocketProtocol.h"

class MiniFigActionHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param camera カメラキャプチャのインスタンス
   */
  MiniFigActionHandler(CameraCapture& camera);

  /**
   * @brief ミニフィグ撮影シーケンスを実行する
   * @param request クライアントからのリクエスト
   * @param response クライアントへのレスポンス
   */
  void execute(const CameraServer::MiniFigActionRequest& request,
               CameraServer::MiniFigActionResponse& response);

 private:
  CameraCapture& camera;
  MiniFigDirectionDetector detector;
  MiniFigDirectionResult firstAttemptResult;  // 1回目の撮影結果を保持する
  int shot_count = 0;                         // 撮影回数カウンター
  const char* filePath = "datafiles/snapshots/";
  const char* uploadFileName = "Fig_FRONT";

  /**
   * @brief 画像からミニフィグの向きを検出する
   * @param frame 入力画像
   * @param result 検出結果
   */
  void detectDirection(cv::Mat& frame, MiniFigDirectionResult& result);
};

#endif  // MINIFIG_ACTION_HANDLER_H
