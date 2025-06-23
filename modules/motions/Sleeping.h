/**
 * @file   Sleeping.h
 * @brief  自タスクスリープ
 * @author molpui0726
 */

#ifndef SLEEPING_H
#define SLEEPING_H

#include "Motion.h"

class Sleeping : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _microSec スリープ時間 (マイクロ秒)
   */
  Sleeping(Robot& _robot, int _microSec);

  /**
   * @brief 自タスクスリープする
   */
  void run();

 private:
  int microSec;  // スリープ時間 (マイクロ秒)
};

#endif
