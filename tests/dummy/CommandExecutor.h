/**
 * @file CommandExecutor.h
 * @brief シェルコマンド実行クラス(CommandExecutor)のダミー
 * @author takuchi17
 */

#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <string>
#include <iostream>

class CommandExecutor {
 public:
  /**
   * @brief テスト用に常に成功(0)を返すダミー実装
   */
  static int exec(const std::string& command)
  {
    std::cout << "Executing command(test): " << command << std::endl;
    return 0;  // 常に成功とする
  }
};

#endif
