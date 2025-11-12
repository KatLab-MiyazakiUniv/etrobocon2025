/**
 * @file   ColorDistanceTwoColorCameraLineTrace.cpp
 * @brief  2色色距離指定カメラライントレース動作
 * @author miyahara046 HaruArima08
 */

#include "ColorDistanceTwoColorCameraLineTrace.h"

ColorDistanceTwoColorCameraLineTrace::ColorDistanceTwoColorCameraLineTrace(
    Robot& _robot, COLOR _targetColor, double _targetDistance, double _targetSpeed,
    int _targetXCoordinate, const PidGain& _pidGain,
    const CameraServer::TwoColorBoundingBoxDetectorRequest& _detectionRequest,
    bool _isStopMotorPower)
  : TwoColorCameraPidTracking(_robot, _targetSpeed, _targetXCoordinate, _pidGain, _detectionRequest,
                              _isStopMotorPower),
    targetColor(_targetColor),
    colorCount(0),
    targetDistance(_targetDistance),
    detectionRequest(_detectionRequest)
{
}

// 色距離指定カメラライントレースの事前条件
bool ColorDistanceTwoColorCameraLineTrace::isMetPreCondition()
{
  // 目標の色がNoneのとき終了する
  if(targetColor == COLOR::NONE) {
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

// 色距離指定カメラライントレースの事前処理
void ColorDistanceTwoColorCameraLineTrace::prepare()
{
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());
}

// 色距離指定カメラライントレースの継続条件
bool ColorDistanceTwoColorCameraLineTrace::isMetContinuationCondition()
{
  // HSV値を取得
  spikeapi::ColorSensor::HSV hsv;
  robot.getColorSensorInstance().getHSV(hsv);

  // 現在の色が目標色と一致していればカウント増加、違えばリセット
  if(ColorJudge::convertHsvToColor(hsv) == targetColor) {
    colorCount++;
  } else {
    colorCount = 0;
  }

  // 走行距離を計算
  double diffDistance
      = fabs(Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                       robot.getMotorControllerInstance().getLeftMotorCount())
             - initDistance);

  // 走行距離が目標距離に到達したときモータが止まる
  if(diffDistance >= targetDistance) {
    return false;
  }

  // 目標色をJUDGE_COUNT回連続で取得したときモータが止まる
  if(colorCount >= JUDGE_COUNT) {
    return false;
  }

  return true;
}