/**
 * @file Button.h
 * @brief ボタンクラス(ダミー)
 * @author HaruArima08
 */

#ifndef BUTTON_H
#define BUTTON_H

namespace spikeapi {
  class Button {
   public:
    /**
     * コンストラクタ
     */
    Button(void) = default;

    /**
     * LEFTボタンが押されているかを確認する
     */
    bool isLeftPressed() const { return isPressedFlag; }

    /**
     * RIGHTボタンが押されているかを確認する
     */
    bool isRightPressed() { return isPressedFlag; }

    /**
     * CENTERボタンが押されているかを確認する
     */
    bool isCenterPressed() { return isPressedFlag; }

    /**
     * BLUETOOTHボタンが押されているかを確認する
     */
    bool isBluetoothPressed() { return isPressedFlag; }
    /**
     * インスタンス生成が正常にできたかどうかを確認するための共通メソッド
     * Buttonでは複数生成が問題ないので、常にfalseを返す
     */
    bool hasError();

   private:
    bool isPressedFlag = true;  // ダミーでは常に押されているとする
  };
}  // namespace spikeapi

#endif
