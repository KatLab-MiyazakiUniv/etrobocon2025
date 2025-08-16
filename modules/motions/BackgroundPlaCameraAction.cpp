/**
 * @file   BackgroundPlaCameraAction.cpp
 * @brief  風景・プラレール撮影動作クラス
 * @author miyahara046
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
    cout << "This is not the correct location for this background direction. Skipping." << endl;
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
  cout << "Requesting BackgroundPla action from server for position: " << position << endl;
  CameraServer::BackgroundPlaActionRequest request;
  request.command = CameraServer::Command::BACKGROUND_PLA_CAMERA_ACTION;
  request.position = position;
  request.threshold = threshold;
  request.minArea = minArea;
  request.roi = roi; // Directly assign cv::Rect

  CameraServer::BackgroundPlaActionResponse response;
  bool success = robot.getSocketClient().executeBackgroundPlaAction(request, response);

  if (success) {
      cout << "Server response: wasDetected=" << response.wasDetected 
           << ", direction=" << response.direction << endl;
      // 1回目の撮影結果だった場合、Robotの状態を更新する
      if (position == 0) {
          BackgroundDirectionResult& result = robot.getBackgroundDirectionResult();
          result.wasDetected = response.wasDetected;
          result.direction = static_cast<BackgroundDirection>(response.direction);
      }
  } else {
      cout << "Failed to get response from server." << endl;
  }

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // 黒線復帰のための回頭をする
  AngleRotation postRotation(robot, postTargetAngle, targetRotationSpeed, !isClockwise);
  postRotation.run();
}
