/**
 * @file   IMURotation.h
 * @brief  IMU角度指定回頭動作
 * @author Hara1274
 */

#ifndef IMU_ROTATION_H
#define IMU_ROTATION_H

#include "Rotation.h"
#include "Pid.h"

class IMURotation : public Rotation {
 public:
  /**
   * コンストラクタ
   * @param _robot       ロボット制御クラスへの参照
   * @param _targetAngle 目標回転角度(deg) 0~360
   * @param _isClockwise 回頭方向 true:時計回り, false:反時計回り
   * @param _angleGain   角度制御ゲイン
   */
  IMURotation(Robot& _robot, int _targetAngle, bool _isClockwise, double _angleGain);
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
   * @brief 継続中にモーターを動的制御する
   */
  void updateMotorControl() override;

 private:
  static constexpr float TOLERANCE = 1.0f;              // 許容誤差
  static constexpr double ANGULAR_VELOCITY_K_P = 0.5;   // 角速度比例ゲイン
  static constexpr double ANGULAR_VELOCITY_K_I = 0.01;  // 角速度積分ゲイン
  static constexpr double ANGULAR_VELOCITY_K_D = 0.0;   // 角速度微分ゲイン
  static constexpr double MIN_MOTOR_POWER = 25.0;       // 最低モータパワー
  int targetAngle;                                      // 目標回転角度(deg) 0~360
  Pid anglePid;                                         // 角度PID制御クラス
  Pid angularVelocityPid;                               // 角速度PID制御クラス
  float currentAngle;                                   // 現在の回頭角度
};

#endif