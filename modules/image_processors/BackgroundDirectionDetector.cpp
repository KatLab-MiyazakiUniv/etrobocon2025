/**
 * @file   BackgroundDirectionDetector.cpp
 * @brief  風景の向きを判定するクラス
 * @author Hara1274 takahashitom
 */

#include "BackgroundDirectionDetector.h"

using json = nlohmann::json;

BackgroundDirectionDetector::BackgroundDirectionDetector() {}

void BackgroundDirectionDetector::detect(BackgroundDirectionResult& result)
{
  int commandResult = CommandExecutor::exec(command);
  if(commandResult != 0) {
    std::cerr << "コマンド実行に失敗しました" << std::endl;
    result.wasDetected = false;
    return;
  }

  // ファイル存在チェック
  std::ifstream file(resultFilePath);
  if(!file.good()) {
    std::cerr << "JSONファイルが存在しません: " << resultFilePath << std::endl;
    result.wasDetected = false;
    return;
  }

  json j;
  try {
    file >> j;
  } catch(const std::exception& e) {
    std::cerr << "JSONパースエラー: " << e.what() << std::endl;
    result.wasDetected = false;
    return;
  }

  // wasDetected を設定
  if(j.contains("wasDetected") && j["wasDetected"].is_boolean()) {
    result.wasDetected = j["wasDetected"].get<bool>();
  } else {
    std::cerr << "wasDetectedが見つからない、または型が不正です" << std::endl;
    result.wasDetected = false;
    return;
  }

  // direction を設定
  if(j.contains("direction") && j["direction"].is_string()) {
    std::string dirStr = j["direction"].get<std::string>();
    if(dirStr == "FRONT")
      result.direction = BackgroundDirection::FRONT;
    else if(dirStr == "RIGHT")
      result.direction = BackgroundDirection::RIGHT;
    else if(dirStr == "BACK")
      result.direction = BackgroundDirection::BACK;
    else if(dirStr == "LEFT")
      result.direction = BackgroundDirection::LEFT;
    else {
      result.wasDetected = false;
      return;
    }
  } else {
    std::cerr << "directionが見つからない、または型が不正です" << std::endl;
    result.wasDetected = false;
    return;
  }
}
