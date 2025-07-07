/**
 * @file   CompositeMotion.h
 * @brief  合成動作クラス
 * @author nishijima515
 */

#ifndef COMPOSITEMOTION_H
#define COMPOSITEMOTION_H

#include "Motion.h"

class CompositeMotion : public Motion {
 public:
  /**
   * コンストラクタ
   */
  CompositeMotion(Robot& _robot);

  /**
   * @brief 合成動作を行う
   * @note オーバーライド必須
   */
  virtual void run() = 0;
};
#endif