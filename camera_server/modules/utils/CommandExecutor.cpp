/**
 * @file   CommandExecutor.cpp
 * @brief  シェルコマンドを実行するクラス
 * @author takuchi17
 */

#include "CommandExecutor.h"
#include <cstdlib>

int CommandExecutor::exec(const std::string& command)
{
  return std::system(command.c_str());
}
