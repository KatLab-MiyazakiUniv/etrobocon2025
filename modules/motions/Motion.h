/**
 * @file   Motion.h
 * @brief  動作の親クラス
 * @author takuchi17
 */

#ifndef MOTION_H
#define MOTION_H

#include "Robot.h"

class Motion {
 public:
  /**
   * コンストラクタ
   * @brief 外部リソースのインスタンスを初期化する
   */
  Motion(Robot& _robot);

  /**
   * @brief 動作を実行する純粋仮想関数
   */
  virtual void run() = 0;

 protected:
  Robot& robot;  // Robotインスタンスのポインタ
};

#endif
