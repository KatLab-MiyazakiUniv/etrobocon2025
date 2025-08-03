/**
 * @file   CommandExecutor.cpp
 * @brief  シェルコマンドを実行するクラスのダミー実装
 * @author takuchi17
 */

#include <iostream>
#include <string>

class CommandExecutor {
 public:
  static int exec(const std::string& command)
  {
    std::cout << "[Dummy CommandExecutor] Simulated exec: " << command << std::endl;
    return 0;
  }
};
