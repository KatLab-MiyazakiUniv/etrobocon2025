/**
 * @file   MotionParserTest.cpp
 * @brief  MotionParserクラスをテストする
 * @author Hara1274
 */

#include "MotionParser.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2025_test {
  // ファイルパスが存在しない場合のテスト
  TEST(MotionParserTest, notCreateMotions)
  {
    Robot robot;
    string csvPath = "../../tests/test_data/NonExistent.csv";  // 存在しないファイルパス
    int targetBrightness = 45;

    vector<Motion*> actualList = MotionParser::createMotions(robot, csvPath, targetBrightness);

    // actualListが空であることを確認
    EXPECT_TRUE(actualList.empty());
  }

  // 複数の異なるモーションタイプを正しく解析するテスト
  TEST(MotionParserTest, createMotions)
  {
    Robot robot;
    string csvPath = "../../tests/test_data/MotionParserTestData.csv";
    int targetBrightness = 45;

    vector<Motion*> actualList = MotionParser::createMotions(robot, csvPath, targetBrightness);
    ASSERT_EQ(actualList.size(), 10);

    ASSERT_NE(dynamic_cast<AngleRotation*>(actualList[0]), nullptr);
    ASSERT_NE(dynamic_cast<DistanceStraight*>(actualList[1]), nullptr);
    ASSERT_NE(dynamic_cast<ColorStraight*>(actualList[2]), nullptr);
    ASSERT_NE(dynamic_cast<DistanceLineTrace*>(actualList[3]), nullptr);
    ASSERT_NE(dynamic_cast<ColorLineTrace*>(actualList[4]), nullptr);
    ASSERT_NE(dynamic_cast<ColorDistanceLineTrace*>(actualList[5]), nullptr);
    ASSERT_NE(dynamic_cast<EdgeChange*>(actualList[6]), nullptr);
    ASSERT_NE(dynamic_cast<Sleeping*>(actualList[7]), nullptr);
    ASSERT_NE(dynamic_cast<Snapshot*>(actualList[8]), nullptr);

    for(Motion* m : actualList) {
      delete m;
    }
  }

  // 無効なコマンドを含む行がスキップされることをテスト
  TEST(MotionParserTest, SkipsInvalidCommand)
  {
    Robot robot;
    string csvPath = "../../tests/test_data/MotionParserInvalidCommandTestData.csv";
    int targetBrightness = 45;

    vector<Motion*> actualList = MotionParser::createMotions(robot, csvPath, targetBrightness);

    // 無効なコマンド（XXX,YYY）は無視され、残りの4つが登録される
    ASSERT_EQ(actualList.size(), 4);

    ASSERT_NE(dynamic_cast<AngleRotation*>(actualList[0]), nullptr);
    ASSERT_NE(dynamic_cast<DistanceStraight*>(actualList[1]), nullptr);
    ASSERT_NE(dynamic_cast<Sleeping*>(actualList[2]), nullptr);
    ASSERT_NE(dynamic_cast<EdgeChange*>(actualList[3]), nullptr);

    for(Motion* m : actualList) {
      delete m;
    }
  }
}  // namespace etrobocon2025_test