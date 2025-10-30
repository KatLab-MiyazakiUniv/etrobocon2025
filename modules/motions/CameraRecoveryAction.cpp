/**
 * @file   CameraRecoveryAction.cpp
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#include "CameraRecoveryAction.h"
#include <thread>
#include <chrono>
#include <iostream>

CameraRecoveryAction::CameraRecoveryAction(
    Robot& _robot, int _angle, double _speed, bool _isClockwise,
    const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
  : CompositeMotion(_robot),
    recoveryAngle(_angle),
    speed(_speed),
    isClockwise(_isClockwise),
    detectionRequest(_detectionRequest)
{
}

void CameraRecoveryAction::run()
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

  AngleRotation rotation(robot, recoveryAngle, speed, isClockwise);
  rotation.run();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // 再検出
  success = client.executeLineDetection(detectionRequest, response);

  if(!success) {
    std::cerr << "通信に失敗しました。" << std::endl;
    return;
  }

  if(response.result.wasDetected) {
    std::cout << "復帰に成功しました。" << std::endl;
  } else {
    std::cout << "復帰できませんでした。" << std::endl;
  }
}
