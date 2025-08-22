// /**
//  * @file AreaMasterTest.cpp
//  * @brief AreaMasterクラスのテスト
//  * @author Hara1274
//  */

// #include "AreaMaster.h"
// #include <gtest/gtest.h>
// #include <gtest/internal/gtest-port.h>
// #include "DummyCameraCapture.h"

// using namespace std;

// namespace etrobocon2025_test {

//   // 左コースでライントレースを行う場合のテスト
//   TEST(AreaMasterTest, RunLineTraceLeft)
//   {
//     DummyCameraCapture cameraCapture;
//     Robot robot(cameraCapture);
//     Area area = Area::LineTrace;
//     bool isLeftCourse = true;
//     int targetBrightness = 45;

//     // 実行前のカウントを保存
//     int beforeRight = robot.getMotorControllerInstance().getRightMotorCount();
//     int beforeLeft = robot.getMotorControllerInstance().getLeftMotorCount();

//     AreaMaster areaMaster(robot, area, isLeftCourse, targetBrightness);
//     areaMaster.run();

//     // 実行後のカウント
//     int afterRight = robot.getMotorControllerInstance().getRightMotorCount();
//     int afterLeft = robot.getMotorControllerInstance().getLeftMotorCount();

//     // カウントが1つでも変わっていれば動作ありとみなす
//     bool isMoved = (beforeRight != afterRight) || (beforeLeft != afterLeft);

//     EXPECT_TRUE(isMoved);
//   }

//   // 右コースでライントレースを行う場合のテスト
//   TEST(AreaMasterTest, RunLineTraceRight)
//   {
//     DummyCameraCapture cameraCapture;
//     Robot robot(cameraCapture);
//     Area area = Area::LineTrace;
//     bool isLeftCourse = false;
//     int targetBrightness = 45;

//     // 実行前のカウントを保存
//     int beforeRight = robot.getMotorControllerInstance().getRightMotorCount();
//     int beforeLeft = robot.getMotorControllerInstance().getLeftMotorCount();

//     AreaMaster areaMaster(robot, area, isLeftCourse, targetBrightness);
//     areaMaster.run();

//     // 実行後のカウント
//     int afterRight = robot.getMotorControllerInstance().getRightMotorCount();
//     int afterLeft = robot.getMotorControllerInstance().getLeftMotorCount();

//     // カウントが1つでも変わっていれば動作ありとみなす
//     bool isMoved = (beforeRight != afterRight) || (beforeLeft != afterLeft);

//     EXPECT_TRUE(isMoved);
//   }
// }  // namespace etrobocon2025_test