/**
 * @file   CameraRecoveryAction.cpp
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#include "CameraRecoveryAction.h"

CameraRecoveryAction::CameraRecoveryAction(Robot& _robot, int _angle, bool _isClockwise,
                                           cv::Scalar _lowerHSV, cv::Scalar _upperHSV)
  : Motion(_robot), recoveryAngle(_angle), isClockwise(_isClockwise)
{
  boundingBoxDetector = std::make_unique<LineBoundingBoxDetector>(_lowerHSV, _upperHSV);
}

void CameraRecoveryAction::run()
{
  // 初期検出を試行
  cv::Mat frame;
  if(!robot.getCameraCaptureInstance().getFrame(frame) || frame.empty()) {
    recoverySuccess = false;
    return;  // フレーム取得失敗
  }

  boundingBoxDetector->detect(frame, result);

  if(result.wasDetected) {
    recoverySuccess = true;
    return;  // 既に検出できている場合は何もしない
  }

  // 指定された角度・方向で回頭
  AngleRotation rotation(robot, recoveryAngle, RECOVERY_SPEED, isClockwise);
  rotation.run();

  // 短時間待機後に検出試行
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // 複数フレームでの安定検出
  for(int i = 0; i < 5; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
    if(!robot.getCameraCaptureInstance().getFrame(frame) || frame.empty()) {
      recoverySuccess = false;
      return;  // フレーム取得失敗
    }
  }
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
