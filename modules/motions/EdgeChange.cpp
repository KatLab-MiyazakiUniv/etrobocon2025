/**
 * @file   EdgeChange.cpp
 * @brief  エッジ切り替え
 * @author miyahara046
 */

#include "EdgeChange.h"

EdgeChange::EdgeChange(Robot& _robot, bool _isLeftEdge) : Motion(_robot), isLeftEdge(_isLeftEdge) {}

void EdgeChange::run()
{
  robot.setIsLeftEdge(isLeftEdge);
}
