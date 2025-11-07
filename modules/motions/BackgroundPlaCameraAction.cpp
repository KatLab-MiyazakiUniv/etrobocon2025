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

BackgroundPlaCameraAction::BackgroundPlaCameraAction(
    Robot& _robot, bool _isClockwise, int _preTargetAngle, int _postTargetAngle, int _basePower,
    double _preTargetDistance, double _postTargetDistance, double _preTargetSpeed,
    double _postTargetSpeed, int _armPower, double _threshold, double _minArea, const cv::Rect _roi,
    int _position, bool _isAbsoluteAngleMode, double _kp, double _ki, double _kd)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    basePower(_basePower),
    preTargetDistance(_preTargetDistance),
    postTargetDistance(_postTargetDistance),
    preTargetSpeed(_preTargetSpeed),
    postTargetSpeed(_postTargetSpeed),
    armPower(_armPower),
    threshold(_threshold),
    minArea(_minArea),
    roi(_roi),
    position(_position),
    isAbsoluteAngleMode(_isAbsoluteAngleMode),
    kp(_kp),
    ki(_ki),
    kd(_kd)
{
}

bool BackgroundPlaCameraAction::isMetPreCondition()
{
  // 初回（position=0）は常に動作する
  if(position == 0) {
    return true;
  }

  // 風景向き判定を失敗した場合、初回でプラレール撮影を行っているため、2回目以降は撮影動作を行わない
  if(!robot.getBackgroundDirectionResult().wasDetected) {
    cout << "風景向き判定を失敗したため、撮影動作は行わない。" << endl;
    return false;
  }

  // 判定成功したが、現在位置が正面位置ではない場合は動作しない
  if(robot.getBackgroundDirectionResult().direction != static_cast<BackgroundDirection>(position)) {
    cout << "現在位置が正面位置ではないため、風景の撮影動作は行わない。" << endl;
    return false;
  }

  // 判定成功し、現在位置が正面位置の場合は動作する
  return true;
}

void BackgroundPlaCameraAction::run()
{
  if(!isMetPreCondition()) return;

  PidGain prePidGain = { kp, ki, kd };
  // 撮影のため回頭
  IMUAngleRotation preRotation(robot, preTargetAngle, basePower, isClockwise, prePidGain,
                               isAbsoluteAngleMode);
  preRotation.run();

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // アームを上げる
  robot.getMotorControllerInstance().setArmMotorPower(armPower);

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // 撮影のため直進
  IMUDistanceStraight preStraight(robot, preTargetDistance, preTargetSpeed, prePidGain);
  preStraight.run();

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

  robot.getMotorControllerInstance().stopWheelsMotor();

  PidGain postPidGain = { kp, ki, kd };

  // 黒線復帰のための後退をする
  IMUDistanceStraight postStraight(robot, postTargetDistance, postTargetSpeed, postPidGain);
  postStraight.run();

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  robot.getMotorControllerInstance().stopWheelsMotor();

  // アームを下げる
  robot.getMotorControllerInstance().setArmMotorPower(-armPower);

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // 黒線復帰のための回頭をする
  IMUAngleRotation postRotation(robot, postTargetAngle, basePower, !isClockwise, postPidGain,
                                isAbsoluteAngleMode);
  postRotation.run();
}
