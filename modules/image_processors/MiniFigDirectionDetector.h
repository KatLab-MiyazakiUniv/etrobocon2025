// /**
//  * @file   MiniFigDirectionDetector.h
//  * @brief  ミニフィグの向きを検出するクラス
//  * @author nishijima515 takahashitom
//  */

// #ifndef MINIFIG_DIRECTION_DETECTOR_H
// #define MINIFIG_DIRECTION_DETECTOR_H

// #include "CommandExecutor.h"
// #include <string>
// #include <iostream>
// #include <fstream>
// #include <filesystem>
// #include <nlohmann/json.hpp>

// // ミニフィグの向きを表す列挙体
// enum class MiniFigDirection { FRONT, RIGHT, BACK, LEFT };

// struct MiniFigDirectionResult {
//   bool wasDetected = false;    // 検出が成功したかどうか
//   MiniFigDirection direction;  // ミニフィグの向きを表す列挙体
// };

// class MiniFigDirectionDetector {
//  public:
//   // コンストラクタ
//   MiniFigDirectionDetector();

//   /**
//    * @brief         入力画像からミニフィグの向きを判定し、結果を result に格納する
//    * @param result  検出結果（構造体）
//    */
//   void detect(MiniFigDirectionResult& result);

//  private:
//   const std::string command = "make -C etrobocon2025/yolo_detection_system/ fig-detection";
//   const std::string resultFilePath =
//   "etrobocon2025/yolo_detection_system/result/fig_result.json";
// };

// #endif  // MINIFIG_DIRECTION_DETECTOR_H