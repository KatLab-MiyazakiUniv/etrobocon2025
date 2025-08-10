/**
 * @file   AngleRotation.h
 * @brief  角度指定回頭動作
 * @author Hara1274
 */

#ifndef ANGLE_ROTATION_H
#define ANGLE_ROTATION_H

#include "Rotation.h"
#include "Mileage.h"
#include "SystemInfo.h"

class AngleRotation : public Rotation {
 public:
  /**
   * コンストラクタ
   * @param _robot       ロボット制御クラスへの参照
   * @param _targetAngle 目標回転角度(deg) 0~360
   * @param _speed       指定する速度（mm/秒）
   * @param _isClockwise 回頭方向 true:時計回り, false:反時計回り
   */
  AngleRotation(Robot& _robot, int _targetAngle, double _speed, bool _isClockwise);
  /**
   * @brief 回頭する
   * @note run() メソッドは Rotation クラスの実装をそのまま使用する
   */
  using Rotation::run;

  /**
   * @brief 回頭動作の事前準備を行う
   */
  void prepare() override;

  /**
   * @brief 回頭する際の事前条件判定をする
   * @return true: 事前条件を満たす,false: 事前条件を満たさない
   */
  bool isMetPreCondition() override;

  /**
   * @brief 回頭する際の継続条件判定をする。返り値がfalseでモーターが止まる
   * @return true: 継続, false: 停止（モーター停止）
   */
  bool isMetContinuationCondition() override;

  /**
   * @brief モーター制御方式を設定する
   */
  void setMotorControl() override;

 private:
  double targetLeftDistance;   // 左モーターの目標移動距離
  double targetRightDistance;  // 右モーターの目標移動距離
  int targetAngle;             // 目標回転角度(deg) 0~360
};

#endif