/**
 * @file PidTest.cpp
 * @brief PidGainクラス, Pidクラスをテストする
 * @author miyahara046
 */

#include "Pid.h"
#include "gtest/gtest.h"

namespace etrobocon2025_test {

  // PidGainの正のゲインがそのまま格納されるかをテスト
  TEST(PidGainTest, PidGainPlus)
  {
    double kp = 0.1;
    double ki = 0.03;
    double kd = 0.612;
    double expected_kp = 0.1;
    double expected_ki = 0.03;
    double expected_kd = 0.612;
    PidGain actualPidGain(kp, ki, kd);
    EXPECT_DOUBLE_EQ(expected_kp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expected_ki, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expected_kd, actualPidGain.kd);
  }

  // PidGainの0のゲインがそのまま格納されるかをテスト
  TEST(PidGainTest, PidGainZero)
  {
    double kp = 0.0;
    double ki = 0.0;
    double kd = 0.0;
    double expected_kp = 0.0;
    double expected_ki = 0.0;
    double expected_kd = 0.0;
    PidGain actualPidGain(kp, ki, kd);
    EXPECT_DOUBLE_EQ(expected_kp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expected_ki, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expected_kd, actualPidGain.kd);
  }

  // PidGainの負のゲインが0に補正されるかをテスト
  TEST(PidGainTest, PidGainMinus)
  {
    double kp = -0.5;
    double ki = -0.2;
    double kd = -0.3;
    PidGain actualPidGain(kp, ki, kd);
    double expected = 0.0;
    EXPECT_DOUBLE_EQ(expected, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expected, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expected, actualPidGain.kd);
  }

  // 基本的なPID計算の結果が正しいかをテスト
  TEST(PidTest, CalculatePid)
  {
    Pid actualPid(0.6, 0.02, 0.03, 70, 100.0, -100.0);
    double currentValue = 20.0;
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (70 - 20) = 50
     * 3. 誤差の積分を計算
     * integral = 0 + (50 + 0) * 0.01 / 2 = 0.25
     * 4. 微分の処理を行う
     * currentDerivative = (50 - 0) / 0.01 = 5000
     *  4.1.  4.1 微分項にローパスフィルタを適用
     * prevDeviat filteredDerivative = 0.8 * 5000 + (1 - 0.8) * 0 = 4000
     * 5. PID制御を計算
     * expected = 0.6 * 50 + 0.02 * 0.25 + 0.03 * 4000 = 150.005
     */
    double expected = 150.005;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  // ゲインが全て0の時のPID計算の結果が正しいかをテスト
  TEST(PidTest, CalculatePidGainZero)
  {
    Pid actualPid(0.0, 0.0, 0.0, 70, 100.0, -100.0);
    double currentValue = 20.0;
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (70 - 20) = 50
     * 3. 誤差の積分を計算
     * integral = 0 + (50 + 0) * 0.01 / 2 = 0.25
     * 4. 微分の処理を行う
     * currentDerivative = (50 - 0) / 0.01 = 5000
     *  4.1 微分項にローパスフィルタを適用
     * prevDeviat filteredDerivative = 0.8 * 5000 + (1 - 0.8) * 0 = 4000
     * 5. PID制御を計算
     * expected = 0.0 * 50 + 0.0 * 0.25 + 0.0 * 4000 = 0.0
     */
    double expected = 0.0;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  // 周期を変更した場合のPID計算結果をテスト
  TEST(PidTest, CalculatePidChangeDelta)
  {
    double DELTA = 0.03;
    Pid actualPid(0.6, 0.02, 0.03, 70, 100.0, -100.0);
    double currentValue = 55.0;
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (70 - 55) = 15
     * 3. 誤差の積分を計算
     * integral = 0 + (15 + 0) * 0.03 / 2 = 0.225
     * 4. 微分の処理を行う
     * currentDerivative = (15 - 0) / 0.03 = 500
     *  4.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * 500 + (1 - 0.8) * 0 = 400
     * 5. PID制御を計算
     * expected = 0.6 * 15 + 0.02 * 0.225 + 0.03 * 400 = 21.0045
     */
    double expected = 21.0045;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, DELTA));
  }

  // 周期に0を渡したとき、内部でデフォルトの0.01が使用されるかをテスト
  TEST(PidTest, CalculatePidChangeDeltaZero)
  {
    Pid actualPid(0.6, 0.02, 0.03, 70, 100.0, -100.0);
    double currentValue = 55.0;
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (70 - 55) = 15
     * 3. 誤差の積分を計算
     * integral = 0 + (15 + 0) * 0.01 / 2 = 0.075
     * 4. 微分の処理を行う
     * currentDerivative = (15 - 0) / 0.01 = 1500
     *  4.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * 1500 + (1 - 0.8) * 0 = 1200
     * 5. PID制御を計算
     * expected = 0.6 * 15 + 0.02 * 0.075 + 0.03 * 1200 = 45.0015
     */
    double expected = 45.0015;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, 0.0));
  }

  // setPidGainでゲイン変更後にPID計算が新しいゲインを使用しているかをテスト
  TEST(PidTest, CalculatePidAfterSetNewGain)
  {
    Pid actualPid(0.6, 0.05, 0.01, 70, 100.0, -100.0);
    double prevValue = 60.0;
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (70 - 60) = 10
     * 3. 誤差の積分を計算
     * integral = 0 + (10 + 0) * 0.01 / 2 = 0.05
     * 4. 微分の処理を行う
     * currentDerivative = (10 - 0) / 0.01 = 1000
     *  4.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * 1000 + (1 - 0.8) * 0 = 800
     * 5. PID制御を計算
     * expected = 0.6 * 10 + 0.05 * 0.005 + 0.01 * 800 = 14.0025
     */
    double expected = 14.0025;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(prevValue));

    actualPid.setPidGain(0.1, 0.2, 0.3);
    double currentValue = 100.0;
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 10
     * 2. 現在の誤差
     * currentDeviation = (70 - 100) = -30
     * 3. 誤差の積分を計算
     * integral = 0.05+ (-30 + 10) * 0.01 / 2 = -0.05
     * 4. 微分の処理を行う
     * currentDerivative = (-30 - 10) / 0.01 = -4000
     *  4.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * -4000 + (1 - 0.8) * 800 = -3040
     * 5. PID制御を計算
     * expected = 0.1 * -30 + 0.2 * -0.1 + 0.3 * -3040 = -915.01
     */
    expected = -915.01;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  // 初回呼び出し時に微分項が正しく計算されるかをテスト
  TEST(PidTest, CalculatePidFirstDerivative)
  {
    Pid pid(0.0, 0.0, 1.0, 100, 100.0, -100.0);
    double currentValue = 90.0;
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (100 - 90) = 10
     * 3. 微分の処理を行う
     * currentDerivative = (10 - 0) / 0.01 = 1000
     *  3.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * 1000 + (1 - 0.8) * 0 = 800
     * 4. PID制御を計算
     * prePID = 0 + 0 + 1.0 * 800 = 800
     */
    double expected = 800.0;
    EXPECT_DOUBLE_EQ(expected, pid.calculatePid(currentValue));
  }

  // 同じ入力を連続して与えたときに微分出力の絶対値が減少することをテスト
  TEST(PidTest, CalculatePidChangeDerivative)
  {
    Pid pid(0.0, 0.0, 1.0, 100, 100.0, -100.0);
    double prevValue = 90.0;
    pid.calculatePid(prevValue);
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (100 - 90) = 10
     * 3. 微分の処理を行う
     * currentDerivative = (10 - 0) / 0.01 = 1000
     *  3.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * 1000 + (1 - 0.8) * 0 = 800
     * 4. PID制御を計算
     * prePID = 0 + 0 + 1.0 * 800 = 800
     */
    double currentValue = 95.0;
    double derivationFirst = pid.calculatePid(currentValue);
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 10
     * 2. 現在の誤差
     * currentDeviation = (100 - 95) = 5
     * 3. 微分の処理を行う
     * currentDerivative = (5 - 10) / 0.01 = -500
     *  3.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * -500 + (1 - 0.8) * 800 = -240
     * 4. PID制御を計算
     * expected_first = 0 + 0 + 1.0 * -240 = -240
     */
    double derivationSecond = pid.calculatePid(currentValue);
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 5
     * 2. 現在の誤差
     * currentDeviation = (100 - 95) = 5
     * 3. 微分の処理を行う
     * currentDerivative = (5 - 5) / 0.01 = 0
     *  3.1 微分項にローパスフィルタを適用
     * filteredDerivative = 0.8 * 0 + (1 - 0.8) * -240 = -48
     * 4. PID制御を計算
     * expected = 0 + 0 + 1.0 * -48 = -48
     */
    double expectedFirst = -240.0;
    double expectedSecond = -48.0;

    EXPECT_DOUBLE_EQ(derivationFirst, expectedFirst);
    EXPECT_DOUBLE_EQ(derivationSecond, expectedSecond);
  }

  // 偏差が0の状態が続くと出力も0を維持するかをテスト
  TEST(PidTest, CalculatePidZeroDeviation)
  {
    Pid pid(1.0, 1.0, 1.0, 50, 100.0, -100.0);
    double expected = 0.0;
    double currentValue = 50.0;
    for(int i = 0; i < 5; ++i) {
      EXPECT_DOUBLE_EQ(expected, pid.calculatePid(currentValue));
    }
  }

  // 積分項が正しく累積されていくかをテスト
  TEST(PidTest, CalculatePidIntegralUpdate)
  {
    Pid pid(0.0, 1.0, 0.0, 100, 100.0, -100.0);
    double currentValue = 0.0;
    double first = pid.calculatePid(currentValue);
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 0
     * 2. 現在の誤差
     * currentDeviation = (100 - 0) = 100
     * 3. 誤差の積分を計算
     * integral = 0 + (100 + 0) * 0.01 / 2 = 0.5
     * 4. PID制御を計算
     * expectedFirst = 0 + 1.0 * 0.5 + 0 = 0.5
     */
    double second = pid.calculatePid(currentValue);
    /** 計算過程
     * 1. 前回の誤差
     * prevDeviation = 100
     * 2. 現在の誤差
     * currentDeviation = (100 - 100) = 0
     * 3. 誤差の積分を計算
     * integral = 0.5 + (0 + 100) * 0.01 / 2 = 1.5
     * 4. PID制御を計算
     * expectedSecond = 0 + 1.0 * 1.5 + 0 = 1.5
     */
    double expectedFirst = 0.5;
    double expectedSecond = 1.5;
    EXPECT_DOUBLE_EQ(expectedFirst, first);
    EXPECT_DOUBLE_EQ(expectedSecond, second);
  }

  // 積分項がmaxIntegralで正しく制限されているかをテスト
  TEST(PidTest, CalculatePidIntegralUpperBound)
  {
    Pid pid(0.0, 1.0, 0.0, 100, 50, -100.0);  // maxIntegral = 50.0
    double currentValue = 0.0;
    for(int i = 0; i < 500; ++i) {  // 5秒間の累積
      pid.calculatePid(currentValue);
    }
    double output = pid.calculatePid(currentValue);
    double expected = 50.0;
    EXPECT_DOUBLE_EQ(output, expected);
  }

  // 積分項がminIntegralで正しく制限されているかをテスト
  TEST(PidTest, CalculatePidIntegralLowerBound)
  {
    Pid pid(0.0, 1.0, 0.0, -100, 100.0, -50.0);  // minIntegral = -50.0
    double currentValue = 0.0;
    for(int i = 0; i < 500; ++i) {  // 5秒間の累積
      pid.calculatePid(currentValue);
    }
    double output = pid.calculatePid(currentValue);
    double expected = -50.0;
    EXPECT_DOUBLE_EQ(output, expected);
  }

  // 積分項の制限をしない時積分項がmaxIntegral = 100.0で正しく制限されているかをテスト
  TEST(PidTest, CalculatePidNotSetIntegralUpperBound)
  {
    Pid pid(0.0, 1.0, 0.0, 100);  // maxIntegral = 100.0
    double currentValue = 0.0;
    for(int i = 0; i < 500; ++i) {  // 5秒間の累積
      pid.calculatePid(currentValue);
    }
    double output = pid.calculatePid(currentValue);
    double expected = 100.0;
    EXPECT_DOUBLE_EQ(output, expected);
  }

  // 積分項の制限をしない時積分項がminIntegral = -100.0で正しく制限されているかをテスト
  TEST(PidTest, CalculatePidNotSetIntegralLowerBound)
  {
    Pid pid(0.0, 1.0, 0.0, -100);  // minIntegral = -100.0
    double currentValue = 0.0;
    for(int i = 0; i < 500; ++i) {  // 5秒間の累積
      pid.calculatePid(currentValue);
    }
    double output = pid.calculatePid(currentValue);
    double expected = -100.0;
    EXPECT_DOUBLE_EQ(output, expected);
  }

}  // namespace etrobocon2025_test
