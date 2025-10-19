/**
 * @file   MiniFigCameraAction.cpp
 * @brief  配置エリアAにおけるミニフィグ撮影動作
 * @author nishijima515 takuchi17
 */

#include "MiniFigCameraAction.h"
#include <thread>
#include <iostream>

using namespace std;

MiniFigCameraAction::MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                                         int _postTargetAngle, int _basePower,
                                         double _backTargetDistance, double _forwardTargetDistance,
                                         double _backSpeed, double _forwardSpeed, int _position,
                                         bool _isAbsoluteMode, double _kp, double _ki, double _kd)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    basePower(_basePower),
    backTargetDistance(_backTargetDistance),
    forwardTargetDistance(_forwardTargetDistance),
    backSpeed(_backSpeed),
    forwardSpeed(_forwardSpeed),
    position(_position),
    isAbsoluteMode(_isAbsoluteMode),
    kp(_kp),
    ki(_ki),
    kd(_kd)
{
}

bool MiniFigCameraAction::isMetPreCondition()
{
  // 2回目以降の撮影で、1回目の結果が「正面以外」であった場合、
  // その向きに応じた撮影場所以外では撮影しない
  if(position != 0 && robot.getMiniFigDirectionResult().wasDetected
     && robot.getMiniFigDirectionResult().direction != static_cast<MiniFigDirection>(position)) {
    cout << "ミニフィグ撮影位置ではありません" << endl;
    return false;
  }
  return true;
}

void MiniFigCameraAction::run()
{
  // 事前条件判定
  if(!isMetPreCondition()) {
    return;
  }

  // 撮影のための回頭をする
  PidGain pidGain = { kp, ki, kd };
  IMUAngleRotation preAR(robot, preTargetAngle, basePower, isClockwise, pidGain, isAbsoluteMode);
  preAR.run();

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // 後退
  DistanceStraight back(robot, backTargetDistance, -backSpeed);
  back.run();

  // サーバーに撮影と判定を依頼
  CameraServer::MiniFigActionRequest request;
  request.command = CameraServer::Command::MINIFIG_CAMERA_ACTION;
  // 綺麗な写真の撮影のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(100));

  CameraServer::MiniFigActionResponse response;
  cout << "サーバーにミニフィグカメラ撮影を依頼: " << position << endl;
  bool success = robot.getSocketClient().executeMiniFigAction(request, response);

  if(success) {
    cout << "ミニフィグ撮影結果: " << response.result.wasDetected
         << ", direction=" << static_cast<int>(response.result.direction) << endl;
    // 1回目の撮影結果だった場合、Robotの状態を更新する
    if(position == 0) {
      MiniFigDirectionResult& result = robot.getMiniFigDirectionResult();
      result.wasDetected = response.result.wasDetected;
      result.direction = response.result.direction;
    }
  } else {
    cout << "サーバーでの撮影に失敗しました。" << endl;
  }

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  robot.getMotorControllerInstance().stopWheelsMotor();
  // 前進
  DistanceStraight forward(robot, forwardTargetDistance, forwardSpeed);
  forward.run();

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  robot.getMotorControllerInstance().stopWheelsMotor();

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // 黒線復帰のための回頭をする
  PidGain postPidGain = { kp, ki, kd };
  IMUAngleRotation postAR(robot, postTargetAngle, basePower, !isClockwise, postPidGain,
                          isAbsoluteMode);
  postAR.run();
}
