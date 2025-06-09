/**
 * @file   StringOperator.h
 * @brief  文字列を操作するstatic関数を持つクラス
 * @author Hara1274
 */
#ifndef STRING_OPERATOR_H
#define STRING_OPERATOR_H

#include <string>

class StringOperator {
 public:
  /**
   * @brief 入力された文字列の末尾の改行を削除する
   * @param string 改行を消したい文字列
   * @return 改行を消した文字列
   */
  static std::string removeEOL(const std::string& str);

 private:
  StringOperator();  // インスタンス化を禁止する
};

#endif