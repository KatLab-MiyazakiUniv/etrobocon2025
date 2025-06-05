/**
 * @file   EdgeChange.cpp
 * @brief  エッジ切り替え
 * @author miyahara046
 */

#include "EdgeChange.h"

EdgeChange::EdgeChange(Robot& _robot, const bool& _isLeftEdge)
  : Motion(_robot), isLeftEdge(_isLeftEdge)
{
}

// エッジを指定した値へ変更
void EdgeChange::run()
{
  robot.setIsLeftEdge(isLeftEdge);
}
