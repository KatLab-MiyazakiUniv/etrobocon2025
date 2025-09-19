/**
 * @file   BackgroundPlaCameraAction.cpp
 * @brief  風景・プラレール撮影動作クラス
 * @author miyahara046 takuchi17
 */

#include "BackgroundPlaCameraAction.h"
#include "AngleRotation.h"
#include <thread>
#include <iostream>

using namespace std;

BackgroundPlaCameraAction::BackgroundPlaCameraAction(Robot& _robot, bool _isClockwise,
                                                     int _preTargetAngle, int _postTargetAngle,
                                                     int _basePower, double _threshold,
                                                     double _minArea, const cv::Rect _roi,
                                                     int _position, double _kp, double _ki,
                                                     double _kd)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    basePower(_basePower),
    threshold(_threshold),
    minArea(_minArea),
    roi(_roi),
    position(_position),
    kp(_kp),
    ki(_ki),
    kd(_kd)
{
}

bool BackgroundPlaCameraAction::isMetPreCondition()
{
  if(position != 0 && robot.getBackgroundDirectionResult().wasDetected
     && robot.getBackgroundDirectionResult().direction
            != static_cast<BackgroundDirection>(position)) {
    cout << "プラレール撮影位置ではありません" << endl;
    return false;
  }
  return true;
}

void BackgroundPlaCameraAction::run()
{
  if(!isMetPreCondition()) return;

  // 撮影のため回頭
  PidGain prePidGain = { kp, ki, kd };
  IMUAngleRotation preRotation(robot, preTargetAngle, basePower, isClockwise, prePidGain);
  preRotation.run();

  // 綺麗な写真の撮影のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(100));

  // サーバーに撮影と判定を依頼
  CameraServer::BackgroundPlaActionRequest request;
  request.command = CameraServer::Command::BACKGROUND_PLA_CAMERA_ACTION;
  request.threshold = threshold;
  request.minArea = minArea;
  request.roi = roi;

  CameraServer::BackgroundPlaActionResponse response;
  cout << "サーバーに風景・プラレールカメラ撮影を依頼: " << position << endl;
  bool success = robot.getSocketClient().executeBackgroundPlaAction(request, response);

  if(success) {
    cout << "風景・プラレール撮影結果: " << response.result.wasDetected
         << ", direction=" << static_cast<int>(response.result.direction) << endl;
    // 1回目の撮影結果だった場合、Robotの状態を更新する
    if(position == 0) {
      BackgroundDirectionResult& result = robot.getBackgroundDirectionResult();
      result.wasDetected = response.result.wasDetected;
      result.direction = response.result.direction;
    }
  } else {
    cout << "サーバーでの撮影に失敗しました。" << endl;
  }

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // 黒線復帰のための回頭をする
  PidGain postPidGain = { kp, ki, kd };
  IMUAngleRotation postRotation(robot, postTargetAngle, basePower, !isClockwise, postPidGain);
  postRotation.run();
}
