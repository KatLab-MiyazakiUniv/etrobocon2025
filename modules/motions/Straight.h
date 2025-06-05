/**
 * @file   Straight.h
 * @brief  直進動作の親クラス
 * @author HaruArima08
 */

#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "Motion.h"
#include "SpeedCalculator.h"

class Straight : public Motion {
 public:
  /**
   * @brief コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _speed 目標速度[mm/s]
   */
  Straight(Robot& _robot, double _speed);

  /**
   * @brief 直進する
   */
  void run();

 protected:
  /**
   * @brief 直進する際の事前条件判定をする
   */
  virtual bool isMetPreCondition() = 0;

  /**
   * @brief 直進する際の事前処理をする
   */
  virtual void prepare() = 0;

  /**
   * @brief 直進する際の動作継続条件判定をする 返り値がtrueの間モーターが回転
   */
  virtual bool isMetContinuationCondition() = 0;

 protected:
  double targetSpeed;  // 目標速度[mm/s]
};

#endif