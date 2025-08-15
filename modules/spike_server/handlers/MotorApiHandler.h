/**
 * @file   MotorApiHandler.h
 * @brief  モーターAPIを処理するハンドラクラス
 * @author takuchi17
 */

#ifndef MOTOR_API_HANDLER_H
#define MOTOR_API_HANDLER_H

#include "ApiHandler.h"  // Include the base ApiHandler
#include "SpikeCommand.h"
#include "Motor.h"

class MotorApiHandler : public ApiHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param client クライアントソケットへのポインタ
   */
  explicit MotorApiHandler(Socket* client);

  /**
   * @brief モーターパワー設定を処理する
   * @param request モーターパワー設定リクエスト
   */
  void handleSetPower(const spike::MotorSetPowerRequest& request);
  /**
   * @brief モーター速度設定を処理する
   * @param request モーター速度設定リクエスト
   */
  void handleSetSpeed(const spike::MotorSetSpeedRequest& request);
  /**
   * @brief 車輪停止を処理する
   */
  void handleStopWheels();
  /**
   * @brief 車輪ブレーキを処理する
   */
  void handleBrakeWheels();
  /**
   * @brief アーム停止を処理する
   */
  void handleStopArm();
  /**
   * @brief アーム保持を処理する
   */
  void handleHoldArm();
  /**
   * @brief モーターカウント取得を処理する
   * @param request モーター取得リクエスト
   */
  spike::Int32Response handleGetCount(const spike::MotorGetRequest& request);
  /**
   * @brief モーターパワー取得を処理する
   * @param request モーター取得リクエスト
   */
  spike::Int32Response handleGetPower(const spike::MotorGetRequest& request);
  /**
   * @brief モーター速度取得を処理する
   * @param request モーター取得リクエスト
   */
  spike::Int32Response handleGetSpeed(const spike::MotorGetRequest& request);

 private:
  spikeapi::Motor rightWheel;  // 右車輪モーターインスタンス
  spikeapi::Motor leftWheel;   // 左車輪モーターインスタンス
  spikeapi::Motor armMotor;    // アームモーターインスタンス
};

#endif  // MOTOR_API_HANDLER_H
