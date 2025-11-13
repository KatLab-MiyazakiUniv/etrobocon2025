/**
 * @file   OrthogonalLineRecoveryAction.cpp
 * @brief  ライン方向の法線に沿って前進して復帰を試みる動作
 */

#include "OrthogonalLineRecoveryAction.h"

OrthogonalLineRecoveryAction::OrthogonalLineRecoveryAction(
    Robot& _robot, int _lineDirectionAngle, int _basePower, const PidGain& _anglePidGain,
    double _orthogonalDistance, double _orthogonalSpeed,
    const CameraServer::BoundingBoxDetectorRequest& _lineDetectionRequest,
    const CameraServer::BoundingBoxDetectorRequest& _pcidsDetectionRequest)
  : CompositeMotion(_robot),
    lineDetectionRequest(_lineDetectionRequest),
    pcidsDetectionRequest(_pcidsDetectionRequest),
    lineDirectionAngle(_lineDirectionAngle),
    basePower(_basePower),
    anglePidGain(_anglePidGain),
    orthogonalDistance(_orthogonalDistance),
    orthogonalSpeed(_orthogonalSpeed)
{
}

void OrthogonalLineRecoveryAction::run()
{
  SocketClient& client = robot.getSocketClient();

  // 初回検出
  CameraServer::BoundingBoxDetectorResponse response;
  bool success = client.executeLineDetection(lineDetectionRequest, response);

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
  success = client.executeLineDetection(lineDetectionRequest, response);

  if(!success) {
    std::cerr << "再検出時の通信に失敗しました。" << std::endl;
    return;
  }

  if(response.result.wasDetected) {
    std::cout << "復帰に成功しました。" << std::endl;
    return;
  }

  // ライン方向で復帰できなかった場合は法線方向でアプローチを試みる
  int orthogonalAngle = lineDirectionAngle + (isClockwise ? 90 : -90);
  if(orthogonalAngle < 0) {
    orthogonalAngle += 360;
  } else if(orthogonalAngle >= 360) {
    orthogonalAngle -= 360;
  }

  double angleBeforeNormalApproach = robot.getIMUControllerInstance().getAngle();
  bool isClockwiseToOrthogonal
      = (std::fmod(orthogonalAngle - angleBeforeNormalApproach + 360.0, 360.0) <= 180.0);
  IMUAngleRotation turnToOrthogonal(robot, orthogonalAngle, basePower, isClockwiseToOrthogonal,
                                    anglePidGain, true);
  turnToOrthogonal.run();

  // 法線方向へ PCIDS で前進しながらライン検出を試みる
  PictureColorDistanceStraight orthogonalApproach(robot, orthogonalDistance, orthogonalSpeed,
                                                  anglePidGain, pcidsDetectionRequest);
  orthogonalApproach.run();

  robot.getMotorControllerInstance().stopWheelsMotor();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // ライン方向に戻って再検出
  double angleBeforeAlign = robot.getIMUControllerInstance().getAngle();
  bool isClockwiseBackToLine
      = (std::fmod(lineDirectionAngle - angleBeforeAlign + 360.0, 360.0) <= 180.0);
  IMUAngleRotation alignToLine(robot, lineDirectionAngle, basePower, isClockwiseBackToLine,
                               anglePidGain, true);
  alignToLine.run();

  success = client.executeLineDetection(lineDetectionRequest, response);
  if(!success) {
    std::cerr << "再検出時の通信に失敗しました。" << std::endl;
    return;
  }

  if(response.result.wasDetected) {
    std::cout << "復帰に成功しました。" << std::endl;
    return;
  }
}
