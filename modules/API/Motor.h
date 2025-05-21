/**
 * @file Motor.h
 * @brief spikeapiのモータラッパークラス
 * @author nishijima515
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "spike/pup/motor.h"
#include "Port.h"

/**
 * SPIKE モータラッパークラス
 * モータの回転方向（時計回り、反時計回り）を指定するための列挙型
 * CLOCKWISEは時計回り、COUNTERCLOCKWISEは反時計回りを表す
 */
class Motor {
 public:
  enum class EDirection {
    CLOCKWISE = PUP_DIRECTION_CLOCKWISE,
    COUNTERCLOCKWISE = PUP_DIRECTION_COUNTERCLOCKWISE,
  };

  /**
   * コンストラクタ
   * @param port PUPポートID
   * @param direction モータの回転方向
   * @param reset_count カウントをリセットするか
   * pup_motor_setup()を複数回呼ぶとハングするため、コンストラクタで一回だけ呼ぶことでエラーを回避する
   */
  Motor(Port port, EDirection direction = EDirection::CLOCKWISE, bool reset_count = true)
    : mHasError(false)
  {
    pupDevicePointer = pup_motor_get_device(static_cast<pbio_port_id_t>(port));
    if(!pupDevicePointer) {
      mHasError = true;
      return;
    }
    pbio_error_t ret
        = pup_motor_setup(pupDevicePointer, static_cast<pup_direction_t>(direction), reset_count);
    if(ret != PBIO_SUCCESS) {
      mHasError = true;
    }
  }

  /**
   * エンコーダをリセットする
   */
  void resetCount() const { pup_motor_reset_count(pupDevicePointer); }

  /**
   * エンコーダの値を取得する
   * @return エンコーダの値 [°]
   */
  int32_t getCount() const { return pup_motor_get_count(pupDevicePointer); }

  /**
   * モータの回転速度を取得する
   * @return 回転速度 [°/秒]
   */
  int32_t getSpeed() const { return pup_motor_get_speed(pupDevicePointer); }

  /**
   * モータの回転速度を設定する
   * @param speed モータの回転速度 [°/秒]
   */
  void setSpeed(int speed) const { pup_motor_set_speed(pupDevicePointer, speed); }

  /**
   * モータのパワー値を取得する
   * @return パワー値（-100 ～ +100）
   */
  int32_t getPower() const { return pup_motor_get_power(pupDevicePointer); }

  /**
   * モータのパワー値を設定する
   * @param power モータのパワー値（-100 ～ +100）
   */
  void setPower(int power) const { pup_motor_set_power(pupDevicePointer, power); }

  /**
   * モータを止める
   */
  void stop() const { pup_motor_stop(pupDevicePointer); }

  /**
   * ブレーキをかけてモータを止める
   */
  void brake() const { pup_motor_brake(pupDevicePointer); }

  /**
   * モータを止めて角度を維持する
   */
  void hold() const { pup_motor_hold(pupDevicePointer); }

  /**
   * モータがストールしているか調べる
   * @return true ストールしている
   * @return false ストールしていない
   */
  bool isStalled() const { return pup_motor_is_stalled(pupDevicePointer); }

  /**
   * モータのデューティ値を下げる
   * @param duty_limit 新しいデューティ値（0-100）
   * @return 元の状態に戻すための最大電圧
   */
  int32_t setDutyLimit(int duty_limit) const
  {
    return pup_motor_set_duty_limit(pupDevicePointer, duty_limit);
  }

  /**
   * モータのデューティ値を元に戻す
   * @param old_value pup_motor_set_duty_limitの戻り値
   */
  void restoreDutyLimit(int old_value) const
  {
    pup_motor_restore_duty_limit(pupDevicePointer, old_value);
  }

  /**
   * インスタンス生成が正常にできたかどうかを確認するための共通メソッド
   * @return true インスタンス生成に失敗した
   * @return false インスタンス生成に成功した
   */
  bool hasError() { return mHasError; }

 private:
  pup_motor_t* pupDevicePointer;  // SPIKE API のモーター構造体へのポインタ
  bool mHasError;                 // インスタンス生成に失敗したかどうかを示すフラグ

};  // class Motor

#endif
