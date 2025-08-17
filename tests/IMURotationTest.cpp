/**
 * @file   IMURotationTest.cpp
 * @brief  IMURotationクラスのテスト
 * @author Hara1274
 */

#include "IMURotation.h"
#include "Robot.h"
#include "SystemInfo.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {

  // 右回頭のテスト
  TEST(IMURotationTest, RunRight)
  {
    Robot robot;

    int angle = 15;
    bool isClockwise = true;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };

    IMURotation imuRotation(robot, angle, isClockwise, anglePidGain);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().calculateOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // ダミーIMUの回転状態を右回頭に設定
    IMUTestControl::rotationStateRef() = 1;

    // 指定した回頭角度を期待値とする
    double expected = angle;

    // 右回頭を実行
    imuRotation.run();

    // IMUから実際の回転角度を取得
    double actual = abs(robot.getIMUControllerInstance().getAngle());

    // TOLERANCEに基づく許容誤差
    double error = 1.0;

    EXPECT_GE(actual, expected - error);
    EXPECT_LE(actual, expected + error);
  }

  // 左回頭のテスト
  TEST(IMURotationTest, RunLeft)
  {
    Robot robot;
    robot.getMotorControllerInstance().resetWheelsMotorPower();

    int angle = 20;
    bool isClockwise = false;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };

    IMURotation imuRotation(robot, angle, isClockwise, anglePidGain);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行（静止状態で実行）
    robot.getIMUControllerInstance().calculateOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // ダミーIMUの回転状態を左回頭に設定
    IMUTestControl::rotationStateRef() = -1;

    // 指定した回頭角度を期待値とする
    double expected = angle;

    // 左回頭を実行
    imuRotation.run();

    // IMUから実際の回転角度を取得
    double actual = abs(robot.getIMUControllerInstance().getAngle());

    // TOLERANCEに基づく許容誤差
    double error = 1.0;

    EXPECT_GE(actual, expected - error);
    EXPECT_LE(actual, expected + error);
  }

  // 左回頭で180度以上回頭するかのテスト
  TEST(IMURotationTest, RunLeftOver180)
  {
    Robot robot;
    robot.getMotorControllerInstance().resetWheelsMotorPower();

    int angle = 185;
    bool isClockwise = false;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };

    IMURotation imuRotation(robot, angle, isClockwise, anglePidGain);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行（静止状態で実行）
    robot.getIMUControllerInstance().calculateOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // ダミーIMUの回転状態を左回頭に設定
    IMUTestControl::rotationStateRef() = -1;

    // 指定した回頭角度を期待値とする
    double expected = angle;

    // 左回頭を実行
    imuRotation.run();

    // IMUから実際の回転角度を取得
    double actual = abs(robot.getIMUControllerInstance().getAngle());

    // TOLERANCEに基づく許容誤差
    double error = 1.0;

    EXPECT_GE(actual, expected - error);
    EXPECT_LE(actual, expected + error);
  }

  // 回頭角度を0に設定したときに回頭をせずに終了するかのテスト
  TEST(IMURotationTest, RunZeroAngle)
  {
    Robot robot;
    robot.getMotorControllerInstance().resetWheelsMotorPower();

    int angle = 0;
    bool isClockwise = true;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };

    IMURotation imuRotation(robot, angle, isClockwise, anglePidGain);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().calculateOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // ダミーIMUの回転状態を右回頭に設定
    IMUTestControl::rotationStateRef() = 1;
    ;

    // 回頭を実行
    imuRotation.run();

    // IMUから実際の回転角度を取得
    double actual = abs(robot.getIMUControllerInstance().getAngle());

    // 回頭しない
    double expected = 0.0;

    EXPECT_EQ(expected, actual);
  }

  // 回頭角度をマイナスに設定したときに回頭をせずに終了するかのテスト
  TEST(IMURotationTest, RunMinusAngle)
  {
    Robot robot;
    robot.getMotorControllerInstance().resetWheelsMotorPower();

    int angle = -1;
    bool isClockwise = true;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };

    IMURotation imuRotation(robot, angle, isClockwise, anglePidGain);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().calculateOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // ダミーIMUの回転状態を右回頭に設定
    IMUTestControl::rotationStateRef() = 1;
    ;

    // 回頭を実行
    imuRotation.run();

    // IMUから実際の回転角度を取得
    double actual = abs(robot.getIMUControllerInstance().getAngle());

    // 回頭しない
    double expected = 0.0;

    EXPECT_EQ(expected, actual);
  }

  // 回頭角度を360度以上に設定したときに回頭をせずに終了するかのテスト
  TEST(IMURotationTest, RunOverAngle)
  {
    Robot robot;
    robot.getMotorControllerInstance().resetWheelsMotorPower();

    int angle = 360;
    bool isClockwise = true;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };  // より保守的な値

    IMURotation imuRotation(robot, angle, isClockwise, anglePidGain);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().calculateOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // ダミーIMUの回転状態を右回頭に設定
    IMUTestControl::rotationStateRef() = 1;
    ;

    // 回頭を実行
    imuRotation.run();

    // IMUから実際の回転角度を取得
    double actual = abs(robot.getIMUControllerInstance().getAngle());

    // 回頭しない
    double expected = 0.0;

    EXPECT_EQ(expected, actual);
  }

  // IMU角度計算が既に開始されている場合にfalseを返すテスト
  TEST(IMURotationTest, ReturnFalseWhenAngleCalculating)
  {
    Robot robot;
    int angle = 30;
    bool isClockwise = true;
    PidGain anglePidGain{ 0.3, 0.005, 0.15 };

    IMURotation imuRotation(robot, angle, isClockwise, anglePidGain);

    // オフセット計算前に静止状態に設定
    IMUTestControl::rotationStateRef() = 0;

    // オフセット計算と補正行列計算を事前実行
    robot.getIMUControllerInstance().calculateOffset();
    robot.getIMUControllerInstance().calculateCorrectionMatrix();

    // 手動でIMU角度計算を開始（既に計算中の状態を作る）
    robot.getIMUControllerInstance().startAngleCalculation();
    EXPECT_TRUE(robot.getIMUControllerInstance().isAngleCalculating());

    // この状態でIMURotationの実行を試行し、falseが返されることを確認
    bool result = imuRotation.run();
    EXPECT_FALSE(result);

    // 手動で停止
    robot.getIMUControllerInstance().stopAngleCalculation();
  }

}  // namespace etrobocon2025_test