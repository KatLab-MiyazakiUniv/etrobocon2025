/**
 * @file IMU.h
 * @brief IMUクラス(ダミー)
 * @author Hara1274
 */

#ifndef SPIKE_CPP_API_IMU_H_
#define SPIKE_CPP_API_IMU_H_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "SystemInfo.h"

// テスト用のIMU用の走行体の状態管理を行う関数
namespace IMUTestControl {
  inline int& rotationStateRef()
  {
    static int state = 0;  // 0:静止, 1:右回頭, -1:左回頭
    return state;  // 回転状態のstatic変数への参照を返す（0:静止, 1:右回頭, -1:左回頭）
  }
}  // namespace IMUTestControl

namespace spikeapi {
  /**
   * SPIKE IMUクラス
   */
  class IMU {
   public:
    /* 加速度 mm/s^2*/
    struct Acceleration {
      float x;
      float y;
      float z;
    };
    /* 角速度 degree/s */
    struct AngularVelocity {
      float x;
      float y;
      float z;
    };

    /**
     * コンストラクタ
     * @param -
     * @return -
     */
    IMU(void)
    {
      srand(12345);  // テスト用固定シード値
    }

    /**
     * IMUから加速度を取得する
     * @param x/y/z軸の加速度を格納するためのAcceleration構造体[mm/s^2]
     * @return -
     */
    void getAcceleration(Acceleration& accel)
    {
      // SPIKEが-45度＋ランダムで傾いている状態をシミュレート
      float randomTilt = (float)(rand() % 21 - 10) * 0.1f;  // ±1度のランダム傾き
      float tiltAngle = -45.0f + randomTilt;                // -45度 + ランダム
      float tiltRad = tiltAngle * DEG_TO_RAD;               // ラジアン変換

      accel.x = 9800.0f * sin(tiltRad);  // 傾きによるX軸成分
      accel.y = 0.0f;                    // Y軸成分なし
      accel.z = 9800.0f * cos(tiltRad);  // 傾きによるZ軸成分
    }

    /**
     * IMUから角速度を取得する
     * @param x/y/z軸の角速度を格納するためのAngularVelocity構造体配列[°/s]
     * @return -
     */
    void getAngularVelocity(AngularVelocity& ang)
    {
      int state = IMUTestControl::rotationStateRef();
      if(state == 1) {
        ang.z = -(float)(rand() % 11);  // 右回頭: -5~-1度/秒
      } else if(state == -1) {
        ang.z = (float)(rand() % 11);  // 左回頭: 1~5度/秒
      } else {
        ang.z = (float)(rand() % 3 - 1) * 0.1f;  // 静止: ±0.1度/秒の小さなノイズ
      }
      ang.x = (float)(rand() % 3 - 1) * 0.1f;  // 小さなノイズ
      ang.y = (float)(rand() % 3 - 1) * 0.1f;  // 小さなノイズ
    }

    /**
     * IMUから温度を取得する
     * @param -
     * @return ダミー用の固定値の温度[℃]
     */
    float getTemperature() const { return 25.0f; }

    /**
     * インスタンス生成が正常にできたかどうかを確認するための共通メソッド
     * IMUでは複数生成が問題ないので、常にfalseを返す
     */
    bool hasError() { return false; }
  };  // class IMU

}  // namespace spikeapi

#endif  // !SPIKE_CPP_API_IMU_H_