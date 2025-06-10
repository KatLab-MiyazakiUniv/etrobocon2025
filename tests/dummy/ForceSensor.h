/**
 * @file ForceSensor.h
 * @brief フォースセンサークラス（ダミー）
 * @author HaruArima08 nishijima515
 */
#ifndef FORCE_SENSOR_H
#define FORCE_SENSOR_H

#include <stdint.h>
#include "Port.h"

namespace spikeapi {
  class ForceSensor {
   public:
    /**
     * コンストラクタ
     * @param port フォースセンサーポート番号
     * @return -
     */
    explicit ForceSensor(EPort port)
    {
      // 実際のセンサーではポートの初期化などを行う
      (void)port;  // 引数を使わないことを明示
    }

    /**
     * フォースセンサで力を測定する
     * @return 力（単位：N）
     */
    float getForce() const { return dummyForce; }

    /**
     * フォースセンサのボタンの移動距離を取得する
     * @return 距離（単位：mm）
     */
    float getDistance() const { return dummyDistance; }

    /**
     * フォースセンサのボタンが押されているかを検出する
     * @param force 押されていると判定する最低限の力（単位：N）
     * @return isPressedFlag
     */
    bool isPressed(float force) const { return isPressedFlag; }

    /**
     * フォースセンサのボタンが触れられているかを検出する
     * @return isTouchedFlag
     */
    bool isTouched() const { return isTouchedFlag; }

    /**
     * インスタンス生成が正常にできたかどうかを確認するための共通メソッド
     * mDeviceがNULLの場合にtrueとなる
     */
    bool hasError() { return mHasError; }

   private:
    bool mHasError;
    float dummyForce = 2.5f;
    float dummyDistance = 1.2f;
    bool isPressedFlag = true;  // ダミーでは常に押されているとする
    bool isTouchedFlag = true;  // ダミーでは常に触れられているとする
  };
}  // namespace spikeapi
#endif
