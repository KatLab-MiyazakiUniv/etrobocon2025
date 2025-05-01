/**
 * @file app.cpp
 * @brief タスクを管理するクラス
 * @author takahashitom
 */

#include "app.h"
#include <iostream>
#include "EtRobocon2025.h"

// メインタスク
void main_task(intptr_t unused)
{
  std::cout << "Hello ETROBO!" << std::endl;
  EtRobocon2025::start();
  ext_tsk();
}
