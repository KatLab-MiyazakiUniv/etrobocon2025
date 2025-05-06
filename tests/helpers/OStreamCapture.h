/**
 * @file   OStreamCapture.h
 * @brief  C++ストリームをキャプチャするためのヘルパークラスの宣言
 * @author takuchi17
 */

#ifndef O_STREAM_CAPTURE_H
#define O_STREAM_CAPTURE_H

#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>

using namespace std;

class OStreamCapture {
 private:
  ostream& targetStream;   // キャプチャ対象の出力ストリーム (例: std::cout)
  stringstream buffer;     // 出力を一時的に保存するバッファ
  streambuf* originalBuf;  // 元のストリームバッファ

 public:
  /**
   * @brief コンストラクタ: ストリームキャプチャの開始 (バッファの差し替え)
   * @param _targetStream キャプチャしたい std::ostream (例: std::cout)
   */
  OStreamCapture(ostream& _targetStream);

  /**
   * @brief デストラクタ: キャプチャ終了とストリームバッファの復元
   */
  ~OStreamCapture();

  /**
   * @brief キャプチャされた出力内容を取得
   * @return 出力された文字列
   */
  string getOutput() const;
};

#endif  // O_STREAM_CAPTURE_H