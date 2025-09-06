/**
 * @file   EdgeChangeTest.cpp
 * @brief  EdgeChangeクラスのテスト
 * @author miyahara046
 */

#include "EdgeChange.h"
#include <gtest/gtest.h>
#include "MockSocketClient.h"

namespace etrobocon2025_test {

  // 初期値が左エッジかのテスト
  TEST(EdgeChangeTest, FirstEdgeIsTrue)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

    bool expected = true;

    // 現在のエッジを取得
    bool actual = robot.getIsLeftEdge();

    // 両方の値がtrueである
    EXPECT_EQ(expected, actual);
  }

  // 右エッジへの切り替えのテスト
  TEST(EdgeChangeTest, RunToFalse)
  {
    MockSocketClient mockSocketClient;
    Robot robot(mockSocketClient);

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