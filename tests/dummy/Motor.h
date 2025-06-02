/**
 * @file Motor.h
 * @brief モータクラス(ダミー)
 * @author nishijima515
 */

#ifndef SPIKE_CPP_API_MOTOR_H_
#define SPIKE_CPP_API_MOTOR_H_

#include <stdint.h>
#include "Port.h"
#include "spikeapi.h"

namespace spikeapi {
  /**
   * SPIKE モータクラス
   */
  class Motor {
   public:
    enum class EDirection {
      CLOCKWISE = 0,
      COUNTERCLOCKWISE = 1,
    };

    /**
     * コンストラクタ
     * @param port PUPポートID
     * @param direction モータの回転方向 初期値はCLOCKWISE(時計回り)
     * @param reset_count カウントをリセットするか　初期値はtrue
     * pup_motor_setup()を複数回呼ぶとハングするため、コンストラクタで一回だけ呼ぶことでエラーを回避する
     */
    Motor(EPort port, EDirection direction = EDirection::CLOCKWISE, bool reset_count = true)
      : mHasError(false)
    {
      // dummyでは常にmhasErrorをtrueにする
      if(1) {
        mHasError = true;
        return;
      }
    }

    /**
     * エンコーダをリセットする
     * @return -
     */
    void resetCount() { count = 0; }

    /**
     * エンコーダの値を取得する
     * @return エンコーダの値 [°]
     */
    int32_t getCount() const { return count; }

    /**
     * モータの回転速度を取得する
     * @return 回転速度 [°/秒]
     */
    int32_t getSpeed() const { return speed; }

    /**
     * モータの回転速度を設定する
     * @param speed モータの回転速度 [°/秒]
     * @return -
     */
    void setSpeed(int speed)
    {
      this->speed = speed;
      count += speed * 0.05;
    }

    /**
     * モータのパワー値を取得する
     * @return パワー値（-100 ～ +100）
     */
    int32_t getPower() const { return power; }

    /**
     * モータのパワー値を設定する
     * @param power モータのパワー値（-100 ～ +100）
     * @return -
     */
    void setPower(int power)
    {
      this->power = power;
      count += power * 0.05;
    }

    /**
     * モータを止める
     * @return -
     */
    void stop()
    {
      speed = 0;
      power = 0;
    }

    /**
     * ブレーキをかけてモータを止める
     * @return -
     */
    void brake()
    {
      speed = 0;
      power = 0;
    }

    /**
     * モータを止めて角度を維持する
     * @return -
     */
    void hold() { speed = 0; }

    /**
     * モータがストールしているか調べる
     * @return true ストールしている
     * @return false ストールしていない
     */
    bool isStalled() const { return is_stalled; }

    /**
     * モータのデューティ値を下げる
     * @param duty_limit 新しいデューティ値（0-100）
     * @return 元の状態に戻すための最大電圧
     */
    int32_t setDutyLimit(int duty_limit)
    {
      int old_value;
      old_value = this->duty_limit;
      this->duty_limit = duty_limit;
      return old_value;
    }

    /**
     * モータのデューティ値を元に戻す
     * @param old_value pup_motor_set_duty_limitの戻り値
     */
    void restoreDutyLimit(int old_value) { duty_limit = old_value; }

    /**
     * インスタンス生成が正常にできたかどうかを確認するための共通メソッド
     */
    bool hasError() { return mHasError; }

   private:
    EPort Port;
    bool mHasError;
    int count = 0;
    int speed = 0;
    int power = 0;
    int duty_limit = 100;
    bool is_stalled = false;

  };  // class Motor
}  // namespace spikeapi

#endif  // !SPIKE_CPP_API_MOTOR_H_
