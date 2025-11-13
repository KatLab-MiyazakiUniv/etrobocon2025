/**
 * @file   SwingCameraRecoveryAction.cpp
 * @brief  カメラ検出失敗時の首振り復帰動作クラス
 * @author HaruArima08
 */

#include "SwingCameraRecoveryAction.h"

SwingCameraRecoveryAction::SwingCameraRecoveryAction(
    Robot& _robot, int _lineDirectionAngle, int _basePower, const PidGain& _anglePidGain,
    int _swingAngle, const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : CompositeMotion(_robot),
    lineDirectionAngle(_lineDirectionAngle),
    basePower(_basePower),
    anglePidGain(_anglePidGain),
    swingAngle(_swingAngle),
    detectionRequest(_detectionRequest)
{
}

void SwingCameraRecoveryAction::run()
{
  SocketClient& client = robot.getSocketClient();

  // 初回検出
  CameraServer::BoundingBoxDetectorResponse response;
  bool success = client.executeLineDetection(detectionRequest, response);

  if(!success) {
    std::cerr << "通信に失敗しました。" << std::endl;
    return;
  }

  if(response.result.wasDetected) {
    std::cout << "ラインを検出できたため、復帰動作の必要はありません。" << std::endl;
    return;
  }

  // 停止処理
  robot.getMotorControllerInstance().stopWheelsMotor();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // IMUの角度計算が開始されているかチェック
  if(!robot.getIMUControllerInstance().getShouldContinueCalculation()) {
    std::cerr << "IS,startで角度計算を事前に開始する必要があります。" << std::endl;
    return;
  }

  // 現在の角度を取得
  double currentAngle = robot.getIMUControllerInstance().getAngle();

  // 目標角度への最短回頭方向を決定
  // fmodで(目標角度 - 現在角度)を0~360度に正規化すると「時計回りで回る場合の角度」を計算
  // この値が180度以下なら時計回りの方が短く、180度より大きければ反時計回りの方が短い
  bool isClockwise = (fmod(lineDirectionAngle - currentAngle + 360.0, 360.0) <= 180.0);

  // ラインの方向へ絶対角度モードで回頭
  IMUAngleRotation turnToLineDirection(robot, lineDirectionAngle, basePower, isClockwise,
                                       anglePidGain, true);
  turnToLineDirection.run();

  // デバッグ用に復帰動作後の画像を保存
  Snapshot snapshot(robot, "recovery_line_direction");
  snapshot.run();

  // 動作安定のためにスリープ
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // 再検出
  success = client.executeLineDetection(detectionRequest, response);

  if(!success) {
    std::cerr << "再検出時の通信に失敗しました。" << std::endl;
    return;
  }

  if(response.result.wasDetected) {
    std::cout << "復帰に成功しました。" << std::endl;
    return;
  }

  // 復帰できなかった場合、首振り動作で検出を試みる
  while(true) {
    // swingAngle分、初回と同じ方向（isClockwise）に相対角度で首を振る
    IMUAngleRotation swing(robot, swingAngle, basePower, isClockwise, anglePidGain, false);
    swing.run();

    // デバッグ用に復帰動作後の画像を保存
    Snapshot snapshot(robot, "recovery_swing");
    snapshot.run();

    // 動作安定のためにスリープ
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // 検出を試みる
    success = client.executeLineDetection(detectionRequest, response);
    if(success && response.result.wasDetected) {
      std::cout << "首振りで復帰に成功しました。" << std::endl;
      return;
    }
  }
}
