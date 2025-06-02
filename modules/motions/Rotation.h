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
   * @param _targetAngle 目標回転角度(deg) 0~360
   * @param _speed       指定する速度値（単位：°/秒）
   * @param _isClockwise 回頭方向 true:時計回り, false:反時計回り
   */
  Rotation(Robot& _robot, int _targetAngle, int _speed, bool _isClockwise);

  /**
   * @brief 回頭する
   */
  void run();

  /**
   * @brief 回頭する際の事前条件判定をする
   * @note オーバーライド必須
   */
  virtual bool isMetPreCondition() = 0;

  /**
   * @brief 回頭する際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @return true: 継続, false: 停止（モーター停止）
   * @note オーバーライド必須
   */
  virtual bool isMetContinuationCondition() = 0;

  /**
   * @brief 実行のログを取る
   * @note オーバーライド必須
   */
  virtual void logRunning() = 0;

 protected:
  int targetAngle;   // 目標回転角度(deg) 0~360
  int speed;         // 指定する速度値（単位：°/秒
  bool isClockwise;  // 回頭方向 true:時計回り, false:反時計回り
  int leftSign;      //   leftSign  = +1 のとき左モーターは順回転、 -1 のとき逆回転
  int rightSign;     //   rightSign = +1 のとき右モーターは順回転、 -1 のとき逆回転
};
#endif