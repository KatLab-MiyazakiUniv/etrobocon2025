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
  // Get SocketClient from Robot
  SocketClient& client = robot.getSocketClient();

  // Initial detection request
  CameraServer::BoundingBoxDetectorResponse response;
  bool success = client.executeLineDetection(detectionRequest, response);

  if(!success) {
    std::cerr << "Failed to get detection response from server." << std::endl;
    return;
  }

  // If already detected, do not perform recovery action
  if(response.result.wasDetected) {
    std::cout << "Detection already successful, no recovery action needed." << std::endl;
    return;
  }

  // Perform rotation
  AngleRotation rotation(robot, recoveryAngle, speed, isClockwise);
  rotation.run();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Small delay after rotation

  // Re-detect after rotation
  success = client.executeLineDetection(detectionRequest, response);

  if(!success) {
    std::cerr << "Failed to get detection response from server after rotation." << std::endl;
    return;
  }

  if(response.result.wasDetected) {
    std::cout << "Recovery action complete, detection successful." << std::endl;
  } else {
    std::cout << "Recovery action failed, detection still unsuccessful." << std::endl;
  }
}
