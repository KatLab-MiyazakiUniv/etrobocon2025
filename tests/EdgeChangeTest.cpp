/**
 * @file   EdgeChangeTest.cpp
 * @brief  EdgeChangeクラスのテスト
 * @author miyahara046
 */

#include "EdgeChange.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {

  // 初期値が左エッジかのテスト: ロボットの初期エッジ設定が左エッジであることを検証する。
  TEST(EdgeChangeTest, FirstEdgeIsTrue)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);

    bool expected = true;

    // 現在のエッジを取得
    bool actual = robot.getIsLeftEdge();

    // 両方の値がtrueである
    EXPECT_EQ(expected, actual);
  }

  // 右エッジへの切り替えのテスト: エッジを右に切り替える動作が正しく行われることを検証する。
  TEST(EdgeChangeTest, RunToFalse)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);

    // 右エッジへ変更
    bool nextEdge = false;
    EdgeChange ec(robot, nextEdge);
    ec.run();

    bool expected = nextEdge;

    // 現在のエッジを取得
    bool actual = robot.getIsLeftEdge();

    // 両方の値がfalseである
    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2025_test