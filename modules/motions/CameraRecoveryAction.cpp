/**
 * @file   CameraRecoveryAction.cpp
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author HaruArima08
 */

#include "CameraRecoveryAction.h"

CameraRecoveryAction::CameraRecoveryAction(
    Robot& _robot, int _angle, double _speed, bool _isClockwise,
    std::unique_ptr<BoundingBoxDetector> _boundingBoxDetector)
  : CompositeMotion(_robot),
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
  std::this_thread::sleep_for(
      std::chrono::milliseconds(33));  // フレームを確実に取得するためのスリープ
  if(!robot.getCameraCaptureInstance().getFrame(frame)) {
    std::cout << "フレーム取得失敗のため終了\n" << std::endl;
    return;
  }

  // 複数フレームから現在の写真を1枚取得する
  for(int i = 0; i < FRAME_NUMBER; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(33));  // フレーム間の待機(33ミリ秒)
    robot.getCameraCaptureInstance().getFrame(frame);
  }

  boundingBoxDetector->detect(frame, result);

  // 既に検出できた場合、復帰動作を行わない
  if(result.wasDetected) {
    std::cout << "復帰動作は行わない\n" << std::endl;
    return;
  }
  // 指定された角度・スピード・方向で回頭復帰
  AngleRotation rotation(robot, recoveryAngle, speed, isClockwise);
  rotation.run();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒待機

  boundingBoxDetector->detect(frame, result);
  if(result.wasDetected) {
    std::cout << "復帰動作完了\n" << std::endl;
    return;  // 検出成功
  }
  std::cout << "復帰動作失敗\n" << std::endl;
}
