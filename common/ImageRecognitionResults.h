#ifndef IMAGE_RECOGNITION_RESULTS_H
#define IMAGE_RECOGNITION_RESULTS_H

#include <cstdint>
#include <opencv2/core/types.hpp>

// ミニフィグの向きを表す列挙体
enum class MiniFigDirection { FRONT, RIGHT, BACK, LEFT };

struct MiniFigDirectionResult {
  bool wasDetected = false;    // 検出が成功したかどうか
  MiniFigDirection direction;  // ミニフィグの向きを表す列挙体
};

// 風景の向きを表す列挙体
enum class BackgroundDirection { FRONT, RIGHT, BACK, LEFT };

struct BackgroundDirectionResult {
  bool wasDetected = false;       // 検出が成功したかどうか
  BackgroundDirection direction;  // 風景の向きを表す列挙体
};

struct BoundingBoxDetectionResult {
  bool wasDetected = false;  // 検出できたかどうか
  cv::Point topLeft;         // 検出した領域の左上の座標
  cv::Point topRight;        // 検出した領域の右上の座標
  cv::Point bottomLeft;      // 検出した領域の左下の座標
  cv::Point bottomRight;     // 検出した領域の右下の座標
};

#endif  // IMAGE_RECOGNITION_RESULTS_H
