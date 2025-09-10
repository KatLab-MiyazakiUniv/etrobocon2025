/**
 * @file   UltrasonicDistanceCameraLineTrace.cpp
 * @brief  超音波距離指定カメラライントレース動作
 * @author nishijima515 miyahara046
 */

#include "UltrasonicDistanceCameraLineTrace.h"

UltrasonicDistanceCameraLineTrace::UltrasonicDistanceCameraLineTrace(
    Robot& _robot, double _targetUltrasonicDistance, double _targetDistance, double _targetSpeed,
    int _targetXCoordinate, const PidGain& _pidGain, std::unique_ptr<BoundingBoxDetector> _detector)
  : CameraPidTracking(_robot, _targetSpeed, _targetXCoordinate, _pidGain, *_detector),
    targetUltrasonicDistance(_targetUltrasonicDistance),
    ultrasonicCount(0),
    targetDistance(_targetDistance),
    detector(std::move(_detector))
{
}

// 超音波距離指定カメラライントレースの事前条件
bool UltrasonicDistanceCameraLineTrace::isMetPreCondition()
{
  // 目標の超音波がNoneのとき終了する
  if(targetUltrasonicDistance <= 0.0) {
    return false;
  }

  // targetSpeed値が0の場合は終了する
  if(targetSpeed == 0.0) {
    return false;
  }

  // targetDistance値が0以下の場合は終了する
  if(targetDistance <= 0.0) {
    return false;
  }

  return true;
}

// 超音波距離指定カメラライントレースの事前処理
void UltrasonicDistanceCameraLineTrace::prepare()
{
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());

  // 超音波カウントを初期化
  ultrasonicCount = 0;
}

// 超音波距離指定カメラライントレースの継続条件
bool UltrasonicDistanceCameraLineTrace::isMetContinuationCondition()
{
  // 超音波センサーの距離を取得
  double ultrasonicDistance = robot.getUltraSonicInstance().getDistance();

  // 現在の色が目標色と一致していればカウント増加、違えばリセット
  if(ultrasonicDistance <= targetUltrasonicDistance && ultrasonicDistance > 0.0) {
    ultrasonicCount++;
  } else {
    ultrasonicCount = 0;
  }

  // (走行距離が目標距離に到達)||(指定された色をJUDGE_COUNT回連続で取得したとき)モータが止まる
  if((fabs(Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                     robot.getMotorControllerInstance().getLeftMotorCount())
           - initDistance)
      >= targetDistance)
     || (ultrasonicCount >= JUDGE_COUNT))
    return false;

  return true;
}