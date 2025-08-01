/**
 * @file   CameraRecoveryAction.cpp
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#include "CameraRecoveryAction.h"

CameraRecoveryAction::CameraRecoveryAction(
    Robot& _robot, int _angle, double _speed, bool _isClockwise,
    std::unique_ptr<BoundingBoxDetector> _boundingBoxDetector)
  : Motion(_robot),
    recoveryAngle(_angle),
    speed(_speed),
    isClockwise(_isClockwise),
    boundingBoxDetector(std::move(_boundingBoxDetector))
{
}

void CameraRecoveryAction::run()
{
  cv::Mat frame;
  // 初期検出確認
  if(!robot.getCameraCaptureInstance().getFrame(frame) || frame.empty()) {
    recoverySuccess = false;
    return;  // フレーム取得失敗
  }

  // 複数フレームでの検出確認
  for(int i = 0; i < FRAME_NUMBER; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(33));  // フレーム間の待機(33ミリ秒)
    robot.getCameraCaptureInstance().getFrame(frame);
  }

  boundingBoxDetector->detect(frame, result);

  // 既に検出できた場合、復帰動作を行わない
  if(result.wasDetected) {
    recoverySuccess = true;
    return;
  }
  // 指定された角度・スピード・方向で回頭復帰
  AngleRotation rotation(robot, recoveryAngle, speed, isClockwise);
  rotation.run();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒待機

  boundingBoxDetector->detect(frame, result);
  if(result.wasDetected) {
    recoverySuccess = true;
    return;  // 検出成功
  }
  recoverySuccess = false;  // 復帰失敗
}

bool CameraRecoveryAction::isRecoverySuccessful() const
{
  return recoverySuccess;
}
