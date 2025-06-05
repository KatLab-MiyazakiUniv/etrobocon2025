/**
 * @file   EdgeChangeTest.cpp
 * @brief  EdgeChangeクラスのテスト
 * @author miyahara046
 */

#include "EdgeChange.h"
#include <gtest/gtest.h>

namespace etrobocon2024_test {

  // 右エッジへの切り替えのテスト
  TEST(EdgeChangeTest, RunFalseToFalse)
  {
    bool nextEdge = false;
    EdgeChange ec(Robot & robot, nextEdge);

    ec.run();

    bool expected = nextEdge;
    bool actual = robot.getIsLeftEdge();

    EXPECT_EQ(expected, actual);
  }

  // 左エッジへの切り替えのテスト
  TEST(EdgeChangeTest, RunToTrue)
  {
    bool nextEdge = true;
    EdgeChange ec(Robot & robot, nextEdge);

    ec.run();

    bool expected = nextEdge;
    bool actual = robot.getIsLeftEdge();

    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2024_test