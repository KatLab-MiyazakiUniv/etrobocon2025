/**
 * @file   BackgroundDirectionDetector.h
 * @brief  風景の向きを判定するクラス
 * @author Hara1274 takahashitom
 */

#ifndef BACKGROUND_DIRECTION_DETECTOR_H
#define BACKGROUND_DIRECTION_DETECTOR_H

#include "CommandExecutor.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

// 風景の向きを表す列挙体
enum class BackgroundDirection { FRONT, RIGHT, BACK, LEFT };

// 結果格納用の構造体
struct BackgroundDirectionResult {
  bool wasDetected = false;       // 検出が成功したかどうか
  BackgroundDirection direction;  // 風景の向きを表す列挙体
};

class BackgroundDirectionDetector {
 public:
  /**
   * コンストラクタ
   */
  BackgroundDirectionDetector();

  /**
   * @brief         入力画像から風景の向きを判定し、結果を result に格納する
   * @param result  検出結果（構造体）
   */
  void detect(BackgroundDirectionResult& result);

 private:
  const std::string command
      = "cd etrobocon2025/yolo_detection_system && make background-detection && cd ../..";
  const std::string resultFilePath
      = "etrobocon2025/yolo_detection_system/result/background_result.json";
};

#endif  // BACKGROUND_DIRECTION_DETECTOR_H