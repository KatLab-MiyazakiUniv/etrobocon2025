/**
 * @file app.cpp
 * @brief タスクを管理するクラス
 * @author takahashitom
 */

#include "app.h"
#include <iostream>
#include "SpikeServer.h"

// メインタスク
void main_task(intptr_t unused)
{
  std::cout << "This is Spike!" << std::endl;
  SpikeServer::start();
  ext_tsk();
}
