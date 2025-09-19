/**
 * @file UltrasonicSensor.h
 * @brief 超音波センサークラス(ダミー)
 * @author nishijima515
 */
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>
#include <stdlib.h>
#include <cstdint>
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
     * @return 距離 (mm)
     */
    int32_t getDistance()
    {
      int distance;
      int index = rand() % 8;
      switch(index) {
        case 0:
          distance = 10;  // 10mm
          break;
        case 1:
          distance = 20;  // 20mm
          break;
        case 2:
          distance = 30;  // 30mm
          break;
        case 3:
          distance = 40;  // 40mm
          break;
        case 4:
          distance = 50;  // 50mm
          break;
        case 5:
          distance = 60;  // 60mm
          break;
        case 6:
          distance = 70;  // 70mm
          break;
        case 7:
          distance = 80;  // 80mm
          break;
      }
      return distance;
    }

   private:
    EPort port;  // 使用するポート
  };

}  // namespace spikeapi
#endif  // ULTRASONIC_SENSOR_H