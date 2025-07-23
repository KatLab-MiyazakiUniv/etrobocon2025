/**
 * @file   PlaCameraActionTest.cpp
 * @brief  PlaCameraActionクラスのテスト
 * @author miyahara046 HaruArima08
 */
#include <gtest/gtest.h>
#include "PlaCameraAction.h"
#include "DummyPlaCameraCapture.h"
#include <filesystem>

// PlaCameraActionが正常に動作するかテスト
TEST(PlaCameraActionTest, RunPlaCameraAction)
{
  DummyPlaCameraCapture dummyPlaCameraCapture;
  dummyPlaCameraCapture.setMotionLikeFrames();

  Robot robot(dummyPlaCameraCapture);
  cv::Rect roi(100, 100, 200, 200);
  PlaCameraAction plaCameraAction(robot, 30.0, 1000.0, roi);

  testing::internal::CaptureStdout();
  plaCameraAction.run();
  std::string output = testing::internal::GetCapturedStdout();
  // 出力内に特定の文字列があるかチェック
  ASSERT_NE(output.find("Filiming time"), std::string::npos);
}
