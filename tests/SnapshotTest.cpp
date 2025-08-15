/**
 * @file   SnapshotTest.cpp
 * @brief  SnapshotTestクラスのテスト
 * @author takuchi17
 */

#include <gtest/gtest.h>
#include "Snapshot.h"

namespace etrobocon2025_test {

  // Snapshotのrun()メソッドが実行できるか確認するテスト
  TEST(SnapshotTest, SnapshotInit)
  {
    SpikeClient spikeClient;
    Robot robot(spikeClient);
    Snapshot snapshot(robot, "test_snapshot");

    // Snapshotのインスタンスが正常に作成されたか確認
    EXPECT_NO_THROW(snapshot.run());
  }
}  // namespace etrobocon2025_test