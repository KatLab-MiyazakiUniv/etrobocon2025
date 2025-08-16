/**
 * @file   Rotation.h
 * @brief  回頭動作の親クラス
 * @author Hara1274
 */

#ifndef ROTATION_H
#define ROTATION_H

#include "Motion.h"

class Rotation : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot       ロボット制御クラスへの参照
   * @param _isClockwise 回頭方向 true:時計回り, false:反時計回り
   */
  Rotation(Robot& _robot, bool _isClockwise);

  /**
   * @brief 回頭する
   */
  void run();

  /**
   * @brief 回頭動作の事前準備を行う
   * @note オーバーライド必須
   */
  virtual void prepare() = 0;

  /**
   * @brief 回頭する際の事前条件判定をする
   * @return true: 事前条件を満たす,false: 事前条件を満たさない
   * @note オーバーライド必須
   */
  virtual bool isMetPreCondition() = 0;

  /**
   * @brief 回頭する際の継続条件判定をする。返り値がfalseでモーターが止まる
   * @return true: 継続, false: 停止（モーター停止）
   * @note オーバーライド必須
   */
  virtual bool isMetContinuationCondition() = 0;

  /**
   * @brief モーター制御方式を設定する
   * @note オーバーライド必須
   */
  virtual void setMotorControl() = 0;

  /**
   * @brief 継続中にモーターを動的制御する
   * @note オーバーライド必須
   */
  virtual void updateMotorControl() {}

 protected:
  bool isClockwise;  // 回頭方向 true:時計回り, false:反時計回り
  int leftSign;      // leftSign  = +1 のとき左モーターは順回転、 -1 のとき逆回転
  int rightSign;     // rightSign = +1 のとき右モーターは順回転、 -1 のとき逆回転
};
#endif