/**
 * @file   CommandExecutor.h
 * @brief  シェルコマンドを実行するクラス
 * @author takuchi17
 */

#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <string>

class CommandExecutor {
 public:
  /**
   * @brief シェルコマンドを実行する
   * @param command 実行するコマンド文字列
   * @return コマンドの終了コード（0: 成功、0以外: 失敗）
   */
  static int exec(const std::string& command);
};

#endif  // COMMAND_EXECUTOR_H
