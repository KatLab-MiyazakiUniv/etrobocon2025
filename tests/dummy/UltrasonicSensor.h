/**
 * @file UltrasonicSensor.h
 * @brief 超音波センサークラス(ダミー)
 * @author nishijima515
 */
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>
#include "Port.h"
#include "spikeapi.h"

namespace spikeapi {

  class UltrasonicSensor {
   public:
    /**
     * @brief コンストラクタ
     * @param port 使用するポート
     */
    explicit UltrasonicSensor(EPort port)
    {
      // 実際のセンサーではポートの初期化などを行う
      (void)port;  // 引数を使わないことを明示
    }

    /**
     * @brief 距離を取得する
     * @return 距離 (cm)
     */
    int32_t getDistance()
    {
      // ダミーでは常に10cmを返す
      return 10;
    }

   private:
    EPort port;  // 使用するポート
  };

}  // namespace spikeapi
#endif  // ULTRASONIC_SENSOR_H