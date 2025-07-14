// /**
//  * @file   EtRobocon2025Test.cpp
//  * @brief  EtRobocon2025クラスのテスト
//  * @author takuchi17
//  */

// #include <gtest/gtest.h>
// #include "EtRobocon2025.h"
// #include "helpers/OStreamCapture.h"

// using namespace std;

// namespace etrobocon2025_test {
//   // start関数が最終的に標準出力に"Hello KATLAB"を出力することを確認するテスト
//   TEST(EtRobocon2025Test, Start)
//   {
//     string expected = "Hello KATLAB\n";

//     // 標準出力をキャプチャ
//     OStreamCapture capture(cout);

//     // start関数を実行
//     EtRobocon2025::start();

//     EXPECT_EQ(expected, capture.getOutput());
//   }
// }  // namespace etrobocon2025_test