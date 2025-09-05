// /**
//  * @file   MiniFigDirectionDetector.cpp
//  * @brief  ミニフィグの向きを検出するクラス
//  * @author nishijima515 takahashitom
//  */

// #include "MiniFigDirectionDetector.h"

// using json = nlohmann::json;

// MiniFigDirectionDetector::MiniFigDirectionDetector() {}

// void MiniFigDirectionDetector::detect(MiniFigDirectionResult& result)
// {
//   int commandResult = CommandExecutor::exec(command);
//   if(commandResult != 0) {
//     std::cerr << "コマンド実行に失敗しました" << std::endl;
//     result.wasDetected = false;
//     return;
//   }

//   // ファイル存在チェック
//   std::ifstream file(resultFilePath);
//   if(!file.good()) {
//     std::cerr << "JSONファイルが存在しません: " << resultFilePath << std::endl;
//     result.wasDetected = false;
//     return;
//   }

//   json j;
//   try {
//     file >> j;
//   } catch(const std::exception& e) {
//     std::cerr << "JSONパースエラー: " << e.what() << std::endl;
//     result.wasDetected = false;
//     return;
//   }

//   // wasDetected を設定
//   if(j.contains("wasDetected") && j["wasDetected"].is_boolean()) {
//     result.wasDetected = j["wasDetected"].get<bool>();
//   } else {
//     std::cerr << "wasDetectedが見つからない、または型が不正です" << std::endl;
//     result.wasDetected = false;
//     return;
//   }

//   // direction を設定
//   if(j.contains("direction") && j["direction"].is_string()) {
//     std::string dirStr = j["direction"].get<std::string>();
//     if(dirStr == "FRONT")
//       result.direction = MiniFigDirection::FRONT;
//     else if(dirStr == "RIGHT")
//       result.direction = MiniFigDirection::RIGHT;
//     else if(dirStr == "BACK")
//       result.direction = MiniFigDirection::BACK;
//     else if(dirStr == "LEFT")
//       result.direction = MiniFigDirection::LEFT;
//     else {
//       result.wasDetected = false;
//       return;
//     }
//   } else {
//     std::cerr << "directionが見つからない、または型が不正です" << std::endl;
//     result.wasDetected = false;
//     return;
//   }
// }
