/**
 * @file   CommandExecutor.cpp
 * @brief  シェルコマンドを実行するクラスのダミー実装
 * @author takuchi17
 */

#include "CommandExecutor.h"
#include <iostream>
#include <string>

int CommandExecutor::exec(const std::string& command)
{
  std::cout << "[Dummy CommandExecutor] Simulated exec: " << command << std::endl;
  return 0;
}
