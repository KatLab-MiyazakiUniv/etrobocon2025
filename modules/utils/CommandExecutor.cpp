/**
 * @file   CommandExecutor.cpp
 * @brief  シェルコマンドを実行するクラス
 * @author takuchi17
 */

#include "CommandExecutor.h"
#include <cstdlib>

int CommandExecutor::exec(const std::string& command)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  return std::system(command.c_str());

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
