/**
 * @file   StringOperator.cpp
 * @brief  文字列を操作するstatic関数を持つクラス
 * @author Hara1274
 */

#include "StringOperator.h"
#include <algorithm>
#include <vector>

std::string StringOperator::removeEOL(const std::string& str)
{
  std::string result = str;
  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
  result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
  return result;
}

std::vector<std::string> StringOperator::split(const std::string& str, char del)
{
  std::vector<std::string> result;
  std::string item;
  for(char ch : str) {
    if(ch == del) {
      result.push_back(item);
      item.clear();
    } else {
      item += ch;
    }
  }
  result.push_back(item);
  return result;
}

StringOperator::StringOperator(){};
