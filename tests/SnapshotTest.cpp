/**
 * @file   SnapshotTest.cpp
 * @brief  SnapshotTestクラスのテスト
 * @author takuchi17
 */

#include <gtest/gtest.h>
#include "Snapshot.h"

namespace etrobocon2025_test {

  // Snapshotの()メソッドが実行できるか確認するテスト
  TEST(SnapshotTest, SnapshotInit)
  {
    Robot robot;
    Snapshot snapshot(robot, "test_snapshot");

    EXPECT_NO_THROW(snapshot.run());
  }

}  // namespace etrobocon2025_test