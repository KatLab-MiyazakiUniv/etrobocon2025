/**
 * @file   PictureColorDistanceStraight.cpp
 * @brief  カメラでの色検出か指定距離で停止する直進動作
 * @author Hara1274
 */

#include "PictureColorDistanceStraight.h"

PictureColorDistanceStraight::PictureColorDistanceStraight(
    Robot& _robot, double _targetDistance, double _targetSpeed, const PidGain& _anglePidGain,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : Straight(_robot, _targetSpeed),
    targetDistance(_targetDistance),
    initialDistance(0.0),
    anglePid(_anglePidGain.kp, _anglePidGain.ki, _anglePidGain.kd, 0.0),
    targetAngle(0.0),
    detectionRequest(_detectionRequest),
    socketClient(nullptr)
{
}

bool PictureColorDistanceStraight::isMetPreCondition()
{
  // 目標速度が0のときは実行しない
  if(targetSpeed == 0.0) {
    return false;
  }

  // 目標距離が0以下のときは実行しない
  if(targetDistance <= 0.0) {
    return false;
  }

  // IMU角度計算がすでに他処理で開始されている場合はエラーとする
  if(robot.getIMUControllerInstance().isAngleCalculating()
     && !robot.getIMUControllerInstance().getShouldContinueCalculation()) {
    std::cerr << "IMU角度計算が既に開始されています。" << std::endl;
    return false;
  }

  return true;
}

void PictureColorDistanceStraight::prepare()
{
  // IMU角度計算がコマンドで開始されていなければ、この動作で計算を開始する
  if(!robot.getIMUControllerInstance().getShouldContinueCalculation()) {
    robot.getIMUControllerInstance().startAngleCalculation();
  }

  // 呼び出し時の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // 走行前の角度を目標角度に設定
  targetAngle = robot.getIMUControllerInstance().getAngle();
}

bool PictureColorDistanceStraight::isMetContinuationCondition()
{
  // 現在の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  // 現在の走行距離が目標走行距離に達した場合falseを返す
  if((fabs(currentDistance - initialDistance) >= targetDistance)) {
    return false;
  }

  // カメラでラインを検出
  CameraServer::BoundingBoxDetectorResponse response;
  bool success = socketClient->executeLineDetection(detectionRequest, response);

  // 通信成功かつライン検出ができた場合のみ終了
  if(success && response.result.wasDetected) {
    return false;
  }

  return true;
}

void PictureColorDistanceStraight::run()
{
  // 事前条件判定が真でないときは終了する
  if(!isMetPreCondition()) {
    return;
  }

  // 事前準備
  prepare();

  // SpeedCalculatorの宣言
  SpeedCalculator speedCalculator(robot, targetSpeed);

  // SocketClientの参照を取得
  socketClient = &robot.getSocketClient();

  // 継続条件を満たしている間繰り返す
  while(isMetContinuationCondition()) {
    // Power値を計算
    double currentRightPower = speedCalculator.calculateRightMotorPower();
    double currentLeftPower = speedCalculator.calculateLeftMotorPower();

    // 角度のズレを補正する
    double currentAngle = robot.getIMUControllerInstance().getAngle();
    double angleError = targetAngle - currentAngle;

    // 角度の誤差を-180度から180度の範囲に正規化
    if(angleError > 180.0) {
      angleError -= 360.0;
    } else if(angleError < -180.0) {
      angleError += 360.0;
    }

    double turningPower = anglePid.calculatePid(angleError, 0.01);

    // 後退時は補正方向を逆にする
    if(targetSpeed < 0) {
      turningPower = -turningPower;
    }

    // モーターにPower値をセット
    robot.getMotorControllerInstance().setRightMotorPower(currentRightPower + turningPower);
    robot.getMotorControllerInstance().setLeftMotorPower(currentLeftPower - turningPower);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒待機
  }

  // モータを停止
  robot.getMotorControllerInstance().stopWheelsMotor();

  // この動作で角度計算を開始した場合のみ、計算を停止
  if(!robot.getIMUControllerInstance().getShouldContinueCalculation()) {
    robot.getIMUControllerInstance().stopAngleCalculation();
  }
}
