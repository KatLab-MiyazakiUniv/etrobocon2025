/**
 * @file   IMUControllerTest.cpp
 * @brief  IMUControllerクラスのテスト
 * @author Hara1274
 */

#include "IMUController.h"
#include "RotationMatrix.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

namespace etrobocon2025_test {

  // コンストラクタのテスト
  TEST(IMUControllerTest, Constructor)
  {
    IMUController imuController;
    // 初期状態では角度計算が停止していることを確認
    EXPECT_FALSE(imuController.isAngleCalculating());
    // 初期角度が0であることを確認
    EXPECT_FLOAT_EQ(0.0f, imuController.getAngle());
  }

  // 生の角速度を取得できるかのテスト
  TEST(IMUControllerTest, GetRawAngularVelocity)
  {
    IMUController imuController;
    float angularVelocity[3];

    // シード値を設定してダミーIMUの値を予測可能にする
    srand(12345);

    // 生の角速度を取得
    imuController.getRawAngularVelocity(angularVelocity);

    // ダミーIMUの実装に基づいて期待値を計算 (rand() % 21 - 10)
    srand(12345);
    float expectedX = (float)(rand() % 21 - 10);
    float expectedY = (float)(rand() % 21 - 10);
    float expectedZ = (float)(rand() % 21 - 10);

    // 期待値と一致することを確認
    EXPECT_FLOAT_EQ(expectedX, angularVelocity[0]);
    EXPECT_FLOAT_EQ(expectedY, angularVelocity[1]);
    EXPECT_FLOAT_EQ(expectedZ, angularVelocity[2]);
  }

  // オフセット計算と補正行列による補正済み角速度計算が正確にできるかのテスト
  TEST(IMUControllerTest, CorrectedAngularVelocity)
  {
    IMUController imuController;

    // オフセット計算を実行
    imuController.calculateOffset();

    // 補正行列を計算
    imuController.calculateCorrectionMatrix();

    // 実際の補正済み角速度を取得（rand()の状態を合わせるためにここで取得する）
    double actualCorrected = imuController.getCorrectedZAxisAngularVelocity();

    // シード値12345でrand()%21-10の値を1000回計算してXYZオフセットの期待値を算出
    srand(12345);
    float expectedOffsetX = 0.0f;
    float expectedOffsetY = 0.0f;
    float expectedOffsetZ = 0.0f;
    for(int i = 0; i < 1000; i++) {
      float x = (float)(rand() % 21 - 10);  // x軸角速度
      float y = (float)(rand() % 21 - 10);  // y軸角速度
      float z = (float)(rand() % 21 - 10);  // z軸角速度
      expectedOffsetX += x;
      expectedOffsetY += y;
      expectedOffsetZ += z;
    }
    expectedOffsetX /= 1000.0f;
    expectedOffsetY /= 1000.0f;
    expectedOffsetZ /= 1000.0f;

    // 補正行列の計算を再現（RotationMatrixクラスを使用）
    // 加速度値を取得（ダミーIMUの実装に合わせて）
    float acc_x = (float)(rand() % 21 - 10);
    float acc_y = (float)(rand() % 21 - 10);
    float acc_z = (float)(rand() % 21 - 10);

    // RotationMatrixクラスを使用して補正行列を計算
    float expectedCorrectionMatrix[3][3];
    RotationMatrix::calculateCorrectionMatrix(acc_x, acc_y, acc_z, expectedCorrectionMatrix);

    // 現在の角速度値を取得して補正済み値を計算
    float current_x = (float)(rand() % 21 - 10);
    float current_y = (float)(rand() % 21 - 10);
    float current_z = (float)(rand() % 21 - 10);

    // 期待される補正済みZ軸角速度（IMUController.cpp:35-36と同じ計算）
    double expectedCorrected = expectedCorrectionMatrix[0][2] * (current_x - expectedOffsetX)
                               + expectedCorrectionMatrix[1][2] * (current_y - expectedOffsetY)
                               + expectedCorrectionMatrix[2][2] * (current_z - expectedOffsetZ);

    // オフセットと補正行列が正しく適用されていることを確認
    EXPECT_DOUBLE_EQ(actualCorrected, expectedCorrected);
  }

  // 角度計算スレッドの開始と停止のテスト
  TEST(IMUControllerTest, AngleCalculationThread)
  {
    IMUController imuController;

    // 初期状態では角度計算が停止していることを確認
    EXPECT_FALSE(imuController.isAngleCalculating());

    // 角度計算を開始
    imuController.startAngleCalculation();
    EXPECT_TRUE(imuController.isAngleCalculating());

    // 角度計算を停止
    imuController.stopAngleCalculation();
    EXPECT_FALSE(imuController.isAngleCalculating());
  }

  // 角度が加算されるかのテスト
  TEST(IMUControllerTest, AngleIntegration)
  {
    IMUController imuController;

    // オフセット計算と補正行列計算を実行
    imuController.calculateOffset();
    imuController.calculateCorrectionMatrix();

    // 角度計算を開始
    imuController.startAngleCalculation();

    // 少し時間を置いて角度が変化することを確認
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    float angle1 = imuController.getAngle();

    // さらに時間を置く
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    float angle2 = imuController.getAngle();

    // 角度計算を停止
    imuController.stopAngleCalculation();

    // 時間経過により角度が変化していることを確認（積分が動作している証拠）
    EXPECT_NE(angle1, angle2);
  }

  // 角度が0にリセットされるかのテスト
  TEST(IMUControllerTest, ResetAngle)
  {
    IMUController imuController;

    // 角度計算を開始して値を変化させる
    imuController.startAngleCalculation();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    // 角度計算を停止
    imuController.stopAngleCalculation();

    // リセット前に角度が0でないことを確認
    EXPECT_NE(0.0f, imuController.getAngle());

    // 角度をリセット
    imuController.resetAngle();

    // 角度が0にリセットされたことを確認
    EXPECT_FLOAT_EQ(0.0f, imuController.getAngle());
  }

  // 安全に二重開始を防ぐ動作のテスト
  TEST(IMUControllerTest, PreventDoubleStart)
  {
    IMUController imuController;

    // 角度計算を開始
    imuController.startAngleCalculation();
    EXPECT_TRUE(imuController.isAngleCalculating());

    // 再度開始を試行
    imuController.startAngleCalculation();

    // 動作していることを確認
    EXPECT_TRUE(imuController.isAngleCalculating());

    // 停止
    imuController.stopAngleCalculation();
    EXPECT_FALSE(imuController.isAngleCalculating());
  }

  // 安全に二重停止を防ぐ動作のテスト
  TEST(IMUControllerTest, PreventDoubleStop)
  {
    IMUController imuController;

    // 角度計算を開始
    imuController.startAngleCalculation();
    EXPECT_TRUE(imuController.isAngleCalculating());

    // 停止
    imuController.stopAngleCalculation();
    EXPECT_FALSE(imuController.isAngleCalculating());

    // 再度停止を試行
    imuController.stopAngleCalculation();

    // 停止状態であることを確認
    EXPECT_FALSE(imuController.isAngleCalculating());
  }

}  // namespace etrobocon2025_test