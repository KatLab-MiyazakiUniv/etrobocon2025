/**
 * @file   ColorSensorClient.h
 * @brief  カラーセンサー操作のためのクライアントAPI
 * @author takuchi17
 */

#ifndef COLORSENSOR_CLIENT_H
#define COLORSENSOR_CLIENT_H

#include "SpikeClient.h"
#include "SpikeCommand.h"

class ColorSensorClient {
 public:
  /**
   * @brief コンストラクタ
   * @param client SpikeClientの参照
   */
  explicit ColorSensorClient(SpikeClient& client);
  /**
   * @brief 色を取得する
   * @return HSV値
   */
  spike::HsvResponse getColor();
  /**
   * @brief 反射光の強さを取得する
   * @return 反射光の強さ
   */
  int32_t getReflection();

 private:
  SpikeClient& spikeClient;  // SpikeClientの参照
};

#endif
