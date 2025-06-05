/**
 * @file   EdgeChange.cpp
 * @brief  エッジ切り替え
 * @author miyahara046
 */

#include "EdgeChange.h"

EdgeChange::edgeChange(bool& _isLeftEdge, bool _nextEdge)
  : isLeftEdge(_isLeftEdge), nextEdge(_nextEdge)
{
}

void EdgeChange::run()
{
  isLeftEdge = nextEdge;
}

bool EdgeChange::getIsLeftEdge() const
{
  return isLeftEdge;
}
