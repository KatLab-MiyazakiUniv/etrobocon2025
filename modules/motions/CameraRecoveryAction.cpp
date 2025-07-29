/**
 * @file   CameraRecoveryAction.cpp
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#include "CameraRecoveryAction.h"
#include <cmath>
#include <chrono>
#include <thread>

CameraRecoveryAction::CameraRecoveryAction(Robot& _robot, BoundingBoxDetector& _boundingBoxDetector,
                                           int _targetXCoordinate)
  : Motion(_robot), boundingBoxDetector(_boundingBoxDetector), targetXCoordinate(_targetXCoordinate)
{
}

void CameraRecoveryAction::run()
{
  // 現在の状態で検出を試行
  cv::Mat frame;
  robot.getCameraCaptureInstance().getFrame(frame);
  boundingBoxDetector.detect(frame, result);

  if(result.wasDetected) {
    double currentX = (result.topLeft.x + result.bottomRight.x) / 2.0;
    if(isDetectedPositionValid(currentX)) {
      recoverySuccess = true;
      return;
    }
  }

  // 初期検出失敗の場合、回頭による復帰を実行
  recoverySuccess = rotationRecovery();
}

bool CameraRecoveryAction::isRecoverySuccessful() const
{
  return recoverySuccess;
}

bool CameraRecoveryAction::rotationRecovery()
{
  for(int angle : RECOVERY_ANGLES) {
    // 左方向で復帰試行
    if(tryRotationDirection(angle, false)) {
      return true;
    }

    // 右方向で復帰試行（左からの2倍角度で対称性を保つ）
    if(tryRotationDirection(angle * 2, true)) {
      return true;
    }
    // 中央位置に戻る（次の角度試行のため）
    AngleRotation centerReturn(robot, angle, RECOVERY_SPEED, false);
    centerReturn.run();

    // 短時間待機
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  return false;  // 全ての試行で復帰失敗
}

bool CameraRecoveryAction::tryRotationDirection(int angle, bool isClockwise)
{
  // 指定方向に回頭
  AngleRotation rotation(robot, angle, RECOVERY_SPEED, isClockwise);
  rotation.run();

  // 短時間待機後に検出試行
  std::this_thread::sleep_for(std::chrono::milliseconds(150));

  cv::Mat frame;
  for(int i = 0; i < 5; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
    robot.getCameraCaptureInstance().getFrame(frame);
  }
  boundingBoxDetector.detect(frame, result);

  if(result.wasDetected) {
    double currentX = (result.topLeft.x + result.bottomRight.x) / 2.0;
    if(isDetectedPositionValid(currentX)) {
      return true;  // 復帰成功
    }
  }

  return false;  // この方向では復帰失敗
}

bool CameraRecoveryAction::isDetectedPositionValid(double detectedX) const
{
  double deviation = std::abs(detectedX - targetXCoordinate);
  return deviation <= MAX_POSITION_DEVIATION;
}