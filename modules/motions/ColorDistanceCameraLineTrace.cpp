/**
 * @file   ColorDistanceCameraLineTrace.cpp
 * @brief  色距離指定カメラライントレース動作
 * @author HaruArima08 nishijima515
 */

#include "ColorDistanceCameraLineTrace.h"

ColorDistanceCameraLineTrace::ColorDistanceCameraLineTrace(
    Robot& _robot, COLOR _targetColor, double _targetDistance, double _targetSpeed,
    int _targetXCoordinate, const PidGain& _pidGain, std::unique_ptr<BoundingBoxDetector> _detector)
  : CameraPidTracking(_robot, _targetSpeed, _targetXCoordinate, _pidGain, *_detector),
    targetColor(_targetColor),
    colorCount(0),
    targetDistance(_targetDistance),
    detector(std::move(_detector))
{
}

// 色指定距離カメラライントレースの事前条件
bool ColorDistanceCameraLineTrace::isMetPreCondition()
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
void ColorDistanceCameraLineTrace::prepare()
{
  // 初期値を代入
  initDistance = Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                           robot.getMotorControllerInstance().getLeftMotorCount());

  // 色カウントを初期化
  colorCount = 0;
}

// 色距離指定カメラライントレースの継続条件
bool ColorDistanceCameraLineTrace::isMetContinuationCondition()
{
  // HSV値を取得
  spikeapi::ColorSensor::HSV hsv;
  robot.getColorSensorInstance().getColor(hsv);

  // 現在の色が目標色と一致していればカウント増加、違えばリセット
  if(ColorJudge::convertHsvToColor(hsv) == targetColor) {
    colorCount++;
  } else {
    colorCount = 0;
  }

  // (走行距離が目標距離に到達)||(指定された色をJUDGE_COUNT回連続で取得したとき)モータが止まる
  if((fabs(Mileage::calculateMileage(robot.getMotorControllerInstance().getRightMotorCount(),
                                     robot.getMotorControllerInstance().getLeftMotorCount())
           - initDistance)
      >= targetDistance)
     || (colorCount >= JUDGE_COUNT))
    return false;

  return true;
}