/**
 * @file   SocketProtocol.h
 * @brief  カメラサーバーとの通信プロトコル定義
 * @author takuchi17
 */

#ifndef SOCKET_PROTOCOL_H
#define SOCKET_PROTOCOL_H

#include "ImageRecognitionResults.h"

#include <cstdint>
#include <opencv2/core/types.hpp>

namespace CameraServer {

  // サーバー側で実行可能なコマンドの列挙体
  enum class Command : uint8_t {
    MINIFIG_CAMERA_ACTION = 0,         // ミニフィグ撮影アクション
    BACKGROUND_PLA_CAMERA_ACTION = 1,  // 背景・プラレール撮影アクション
    TAKE_SNAPSHOT = 2,                 // スナップショット撮影アクション
    LINE_DETECTION = 3,                // ライン検出
    SHUTDOWN = 255                     // サーバーシャットダウン
  };

  // ミニフィグ撮影アクションのリクエストデータ構造
  struct MiniFigActionRequest {
    Command command = Command::MINIFIG_CAMERA_ACTION;  // MINIFIG_CAMERA_ACTIONを期待
  };

  // ミニフィグ撮影アクションのレスポンスデータ構造
  struct MiniFigActionResponse {
    MiniFigDirectionResult result;  // ミニフィグの向き検出結果
  };

  // 背景・プラレール撮影アクションのリクエストデータ構造
  struct BackgroundPlaActionRequest {
    Command command = Command::BACKGROUND_PLA_CAMERA_ACTION;  // BACKGROUND_PLA_CAMERA_ACTIONを期待
    double threshold;                                         // 動体検出の閾値
    double minArea;                                           // 動体とみなす最小面積
    cv::Rect roi;                                             // 動体検出を行うROI
  };

  // 背景・プラレール撮影アクションのレスポンスデータ構造
  struct BackgroundPlaActionResponse {
    BackgroundDirectionResult result;  // 風景の向き検出結果
  };

  // バウンディングボックス検出のリクエストデータ構造
  struct BoundingBoxDetectorRequest {
    Command command = Command::LINE_DETECTION;  // LINE_DETECTIONを期待
    cv::Scalar lowerHSV;                        // 検出する色の下限HSV値
    cv::Scalar upperHSV;                        // 検出する色の上限HSV値
    cv::Rect roi;                               // 検出を行うROI
    cv::Size resolution;                        // カメラ画像の解像度
  };

  // バウンディングボックス検出のレスポンスデータ構造
  struct BoundingBoxDetectorResponse {
    BoundingBoxDetectionResult result;  // バウンディングボックス検出結果
  };

  // スナップショット撮影アクションのリクエストデータ構造
  struct SnapshotActionRequest {
    Command command = Command::TAKE_SNAPSHOT;  // TAKE_SNAPSHOTを期待
    char fileName[64];                         // 保存するファイル名
  };

  // スナップショット撮影アクションのレスポンスデータ構造
  struct SnapshotActionResponse {
    bool success;  // 撮影が成功したかどうか
  };

}  // namespace CameraServer

#endif  // SOCKET_PROTOCOL_H
