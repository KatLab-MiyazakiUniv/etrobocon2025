/**
 * @file   EdgeChangeTest.cpp
 * @brief  EdgeChangeクラスのテスト
 * @author miyahara046
 */

#include "EdgeChange.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {

  // 右エッジへの切り替えのテスト
  TEST(EdgeChangeTest, RunToFalse)
  {
    // 右エッジへ変更
    bool nextEdge = false;
    EdgeChange ec(Robot & robot, nextEdge);
    ec.run();

    bool expected = nextEdge;

    // 現在のエッジを取得
    bool actual = robot.getIsLeftEdge();

    // 両方の値がfalseである
    EXPECT_EQ(expected, actual);
  }

  // 左エッジへの切り替えのテスト
  TEST(EdgeChangeTest, RunToTrue)
  {
    // 左エッジへの変更
    bool nextEdge = true;
    EdgeChange ec(Robot & robot, nextEdge);
    ec.run();

    bool expected = nextEdge;

    // 現在のエッジを取得
    bool actual = robot.getIsLeftEdge();

    // 両方の値がtrueである
    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2025_test