/**
 * @file   StringOperator.cpp
 * @brief  文字列を操作するstatic関数を持つクラス
 * @author Hara1274
 */

#include "StringOperator.h"

std::string StringOperator::removeEOL(const std::string& str)
{
  if(str.empty()) {
    return str;
  }

  size_t endIndex = str.size() - 1;

  // 末尾の改行コードを削除（LF,CR,CR+LF対応）
  // LFの場合
  if(str[endIndex] == 0x0a) {
    endIndex--;
  }
  // CRの場合
  if(endIndex != std::string::npos && str[endIndex] == 0x0d) {
    endIndex--;
  }

  // substrの第2引数は長さなので +1 必要
  return str.substr(0, endIndex + 1);
}