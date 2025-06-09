/**
 * @file AreaMasterTest.cpp
 * @brief AreaMasterクラスのテスト
 * @author Hara1274
 */

#include "AreaMaster.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2025_test {

  // 左コースでライントレースを行う場合のテスト
  TEST(AreaMasterTest, runLineTraceLeft)
  {
    Robot robot;
    Area area = Area::LineTrace;
    bool isLeftCourse = true;
    int targetBrightness = 45;

    class AreaMaster areaMaster(robot, area, isLeftCourse, targetBrightness);
    EXPECT_NO_THROW({ areaMaster.run(); });
  }

  // 右コースでライントレースを行う場合のテスト
  TEST(AreaMasterTest, runLineTraceRight)
  {
    Robot robot;
    Area area = Area::LineTrace;
    bool isLeftCourse = false;
    int targetBrightness = 45;

    class AreaMaster areaMaster(robot, area, isLeftCourse, targetBrightness);
    EXPECT_NO_THROW({ areaMaster.run(); });
  }
}  // namespace etrobocon2025_test