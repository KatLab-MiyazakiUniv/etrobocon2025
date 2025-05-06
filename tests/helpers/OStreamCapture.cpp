/**
 * @file   OStreamCapture.cpp
 * @brief  C++ストリームをキャプチャするためのヘルパークラスの実装
 * @author takuchi17
 */

#include "OStreamCapture.h"

OStreamCapture::OStreamCapture(ostream& _targetStream)
  : targetStream(_targetStream), originalBuf(_targetStream.rdbuf())
{
  targetStream.rdbuf(buffer.rdbuf());
}

OStreamCapture::~OStreamCapture()
{
  targetStream.rdbuf(originalBuf);
}

string OStreamCapture::getOutput() const
{
  return buffer.str();
}
