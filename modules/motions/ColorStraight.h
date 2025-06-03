/**
 * @file   ColorStraight.cpp
 * @brief  色まで直進するクラス
 * @author HaruArima08
 */

#ifndef COLOR_STRAIGHT_H
#define COLOR_STRAIGHT_H

#include "Straight.h"
#include "ColorJudge.h"

class ColorStraight : public Straight {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _targetColor 目標色
   * @param _speed       目標速度[mm/s]
   */
  ColorStraight(Robot& _robot, COLOR _targetColor, double _speed);

 protected:
  /**
   * @brief 直進する際の事前条件判定をする
   */
  bool isMetPreCondition() override;

  /**
   * @brief 直進する際の事前処理をする
   */
  void prepare() override;

  /**
   * @brief 直進する際の動作継続条件判定をする 返り値がtrueの間モーターが回転
   */
  bool isMetContinuationCondition() override;

 private:
  static constexpr int JUDGE_COUNT = 3;
  int colorCount = 0;
  COLOR targetColor;  // 目標色
};

#endif