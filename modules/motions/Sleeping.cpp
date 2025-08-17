/**
 * @file   Sleeping.cpp
 * @brief  自タスクスリープ
 * @author molpui0726
 */

#include "Sleeping.h"
#include <thread>
#include <chrono>

Sleeping::Sleeping(Robot& _robot, int _milliSec) : Motion(_robot), milliSec(_milliSec) {}

void Sleeping::run()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliSec));
}
