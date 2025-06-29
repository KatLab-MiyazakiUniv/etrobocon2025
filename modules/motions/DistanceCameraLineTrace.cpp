/**
 * @file   DistanceCameraLineTrace.cpp
 * @brief  指定距離カメラライントレース動作
 * @author miyahara046 HaruArima08
 */

#include "DistanceCameraLineTrace.h"

DistanceCameraLineTrace::DistanceCameraLineTrace(Robot& _robot, double _targetDistance,
                                                 double _targetSpeed, int _targetXCoordinate,
                                                 const PidGain& _pidGain,
                                                 std::unique_ptr<BoundingBoxDetector> _detector)
  : CameraPidTracking(_robot, _targetSpeed, _targetXCoordinate, _pidGain, *_detector),
    targetDistance(_targetDistance),
    detector(std::move(_detector))
{
}

// 指定距離カメラライントレースの事前条件
bool DistanceCameraLineTrace::isMetPreCondition()
{
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

// 指定距離カメラライントレースの事前処理
void DistanceCameraLineTrace::prepare()
{
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());
}

// 指定距離カメラライントレースの継続条件
bool DistanceCameraLineTrace::isMetContinuationCondition()
{
  // // フレーム取得をJUDGE_COUNT回数以上失敗
  // cv::Mat frame;
  // if(!robot.getCameraCaptureInstance().getFrame(frame) || frame.empty()) {
  //   frameCount++;
  //   if(frameCount >= JUDGE_COUNT) {
  //     return false;
  //   }
  // } else {
  //   frameCount = 0;
  // }
  // 走行距離が目標距離に到達
  if(fabs(Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                    robot.getMotorControllerInstance().getLeftMotorCount())
          - initDistance)
     >= targetDistance)
    return false;

  return true;
}