/**
 * @file   ColorSensorApiHandler.h
 * @brief  カラーセンサーAPIを処理するハンドラクラス
 * @author takuchi17
 */

#ifndef COLOR_SENSOR_API_HANDLER_H
#define COLOR_SENSOR_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "ColorSensor.h"

class ColorSensorApiHandler : public ApiHandler {  // Inherit from ApiHandler
 public:
  /**
   * @brief コンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  explicit ColorSensorApiHandler(Socket* client);

  /**
   * @brief 反射値取得を処理する
   */
  void handleGetReflection();
  /**
   * @brief HSVカラー値取得を処理する
   */
  void handleGetColorHsv();

 private:
  spikeapi::ColorSensor colorSensor;  // カラーセンサーインスタンス
};

#endif  // COLOR_SENSOR_API_HANDLER_H
