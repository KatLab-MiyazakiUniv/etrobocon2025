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
                                                     double _targetRotationSpeed, double _threshold,
                                                     double _minArea, const cv::Rect _roi,
                                                     int _position)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    targetRotationSpeed(_targetRotationSpeed),
    threshold(_threshold),
    minArea(_minArea),
    roi(_roi),
    position(_position)
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
  AngleRotation preRotation(robot, preTargetAngle, targetRotationSpeed, isClockwise);
  preRotation.run();

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // サーバーに撮影と判定を依頼
  CameraServer::BackgroundPlaActionRequest request;
  request.command = CameraServer::Command::BACKGROUND_PLA_CAMERA_ACTION;
  request.position = position;
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
  AngleRotation postRotation(robot, postTargetAngle, targetRotationSpeed, !isClockwise);
  postRotation.run();
}
