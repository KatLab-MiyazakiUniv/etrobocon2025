/**
 * @file   CameraControllerTest.cpp
 * @brief  Cameraクラスのテスト
 * @author HaraNaruki
 */

#include <gtest/gtest.h>
#include "../usb_camera/CameraCapure.h"
#include "helpers/OStreamCapture.h"

using namespace std;

namespace CameraCapure_test {
  // start関数が最終的に標準出力に"Hello KATLAB"を出力することを確認するテスト
  TEST(CameraCapureTest, Start)
  {
    string expected = "Camera started.\n";

    // 標準出力をキャプチャ
    OStreamCapture capture(cout);


    // start関数を実行
    CameraCapture::start();

    EXPECT_EQ(expected, capture.getOutput());
  }
}  // namespace etrobocon2025_test