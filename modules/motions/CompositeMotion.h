/**
 * @file   CompositeMotion.h
 * @brief  合成動作の親クラス
 * @author nishijima515
 */

#ifndef COMPOSITE_MOTION_H
#define COMPOSITE_MOTION_H

#include "Motion.h"

class CompositeMotion : public Motion {
 public:
  /**
   * コンストラクタ
   * @brief 外部リソースのインスタンスを初期化する
   * @param _robot ロボット制御クラスへの参照
   */
  CompositeMotion(Robot& _robot);

  /**
   * @brief 合成動作を行う
   * @note オーバーライド必須
   */
  virtual void run() = 0;
};
#endif