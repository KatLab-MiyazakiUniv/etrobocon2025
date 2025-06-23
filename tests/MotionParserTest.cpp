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
  TEST(MotionParserTest, NotCreateMotions)
  {
    Robot robot;
    string csvPath = "../../tests/test_data/NonExistent.csv";  // 存在しないファイルパス
    int targetBrightness = 45;
    CameraCapture cameraCapture;

    vector<Motion*> actualList
        = MotionParser::createMotions(robot, csvPath, targetBrightness, cameraCapture);

    // actualListが空であることを確認
    EXPECT_TRUE(actualList.empty());
  }

  // 複数の異なるモーションタイプを正しく作成するテスト
  TEST(MotionParserTest, CreateMotions)
  {
    Robot robot;
    string csvPath = "../../tests/test_data/MotionParserTestData.csv";
    int targetBrightness = 45;
    CameraCapture cameraCapture;

    vector<Motion*> actualList
        = MotionParser::createMotions(robot, csvPath, targetBrightness, cameraCapture);
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
    ASSERT_NE(dynamic_cast<Snapshot*>(actualList[9]), nullptr);

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
    CameraCapture cameraCapture;

    vector<Motion*> actualList
        = MotionParser::createMotions(robot, csvPath, targetBrightness, cameraCapture);

    // 無効なコマンド（XXX,YYY）は無視され、残りの4つが格納される
    ASSERT_EQ(actualList.size(), 4);

    ASSERT_NE(dynamic_cast<AngleRotation*>(actualList[0]), nullptr);
    ASSERT_NE(dynamic_cast<DistanceStraight*>(actualList[1]), nullptr);
    ASSERT_NE(dynamic_cast<Sleeping*>(actualList[2]), nullptr);
    ASSERT_NE(dynamic_cast<EdgeChange*>(actualList[3]), nullptr);

    for(Motion* m : actualList) {
      delete m;
    }
  }

  // 実際のLineTraceLeftファイルで実行できるかのテスト
  TEST(MotionParserTest, ParseLineTraceLeftFile)
  {
    Robot robot;
    string csvPath = "../../datafiles/commands/LineTraceLeft.csv";
    int targetBrightness = 45;
    CameraCapture cameraCapture;

    ifstream file(csvPath);
    if(!file.is_open()) {
      ASSERT_TRUE(file.is_open());
    }

    int lines = 0;
    string line;
    while(getline(file, line)) {
      ++lines;
    }

    vector<Motion*> actualList
        = MotionParser::createMotions(robot, csvPath, targetBrightness, cameraCapture);

    // ファイルの行数分だけ、motionのリストに格納されたことを確認
    ASSERT_EQ(actualList.size(), lines);

    for(Motion* m : actualList) {
      delete m;
    }
  }

  // 実際のLineTraceRightファイルで実行できるかのテスト
  TEST(MotionParserTest, ParseLineTraceRightFile)
  {
    Robot robot;
    string csvPath = "../../datafiles/commands/LineTraceRight.csv";
    int targetBrightness = 45;
    CameraCapture cameraCapture;

    ifstream file(csvPath);
    if(!file.is_open()) {
      ASSERT_TRUE(file.is_open());
    }

    int lines = 0;
    string line;
    while(getline(file, line)) {
      ++lines;
    }

    vector<Motion*> actualList
        = MotionParser::createMotions(robot, csvPath, targetBrightness, cameraCapture);

    // ファイルの行数分だけ、motionのリストに格納されたことを確認
    ASSERT_EQ(actualList.size(), lines);

    for(Motion* m : actualList) {
      delete m;
    }
  }

}  // namespace etrobocon2025_test
