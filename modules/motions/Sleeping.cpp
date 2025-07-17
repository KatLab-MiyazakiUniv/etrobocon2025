/**
 * @file   Sleeping.cpp
 * @brief  自タスクスリープ
 * @author molpui0726
 */

#include "Sleeping.h"

Sleeping::Sleeping(Robot& _robot, int _microSec) : Motion(_robot), microSec(_microSec) {}

void Sleeping::run()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
