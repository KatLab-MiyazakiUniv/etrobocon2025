/**
 * @file   Snapshot.h
 * @brief  写真を撮影して保存するクラス
 * @author takuchi17
 */

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

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
  Robot& robot;  // Robotインスタンスの参照
};

#endif
