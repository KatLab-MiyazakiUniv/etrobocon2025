/**
 * @file ForceSensor.h
 * @brief フォースセンサークラス（ダミー）
 * @author nishijima515
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
     * @param port PUPポートID
     */
    ForceSensor(EPort port) {}
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
     * @return true 押されている
     * @return false 押されていない
     */
    bool isPressed(float force) const { return isPressedFlag; }
    /**
     * フォースセンサのボタンが触れられているかを検出する
     * @return true 触れられている
     * @return false 触れられていない
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
  };  // class ForceSensor
}  // namespace spikeapi
#endif  // !FORCE_SENSOR_H
