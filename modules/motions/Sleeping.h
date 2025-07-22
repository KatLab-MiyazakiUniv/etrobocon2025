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
   * @param _robot ロボットのインスタンス
   * @param _milliSec スリープ時間 (ミリ秒)
   */
  Sleeping(Robot& _robot, int _milliSec);

  /**
   * @brief 自タスクスリープする
   * @note オーバーライド必須
   */
  void run() override;

 private:
  int milliSec;  // スリープ時間 (ミリ秒)
};

#endif
