/**
 * @file Button.h
 * @brief ボタンクラス(ダミー)
 * @author HaruArima08 nishijima515
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
     * LEFTボタンが押されているかをランダムに判定
     */
    bool isLeftPressed() const { return std::rand() % 2 == 0; }

    /**
     * RIGHTボタンが押されているかをランダムに判定
     */
    bool isRightPressed() const { return std::rand() % 2 == 0; }

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
