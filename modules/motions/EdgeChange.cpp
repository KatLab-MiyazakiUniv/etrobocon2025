/**
 * @file   EdgeChange.cpp
 * @brief  エッジ切り替え
 * @author miyahara046
 */

#include "EdgeChange.h"

// エッジを指定した値へ変更
void EdgeChange::run()
{
  robot.setIsLeftEdge(isLeftEdge);
}
