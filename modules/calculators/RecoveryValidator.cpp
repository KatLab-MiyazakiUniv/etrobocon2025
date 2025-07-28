/**
 * @file   RecoveryValidator.cpp
 * @brief  復帰動作時の位置変化検証クラス
 * @author HaruArima08
 */

#include "RecoveryValidator.h"
#include <cmath>

RecoveryValidator::RecoveryValidator(double _maxCenterXChange)
  : maxCenterXChange(_maxCenterXChange), prevXCoordinate(-1.0), hasPreRecoveryData(false)
{
}

void RecoveryValidator::savePreRecoveryPosition(double centerX)
{
  prevXCoordinate = centerX;
  hasPreRecoveryData = true;
}

bool RecoveryValidator::isPositionChangeValid(double currentX)
{
  if(!hasPreRecoveryData) {
    return true;  // 復帰前データがない場合は妥当とみなす
  }

  double positionChange = std::abs(currentX - prevXCoordinate);
  return positionChange <= maxCenterXChange;
}

void RecoveryValidator::clear()
{
  prevXCoordinate = -1.0;
  hasPreRecoveryData = false;
}