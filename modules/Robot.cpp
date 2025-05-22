/**
 * @file   Robot.cpp
 * @brief  外部リソースのインスタンスを管理するクラス
 * @author takuchi17
 */

#include "Robot.h"

Robot::Robot() : motorController(), cameraCapture() /*, colorSensor()*/ {}

MotorController& Robot::getMotorControllerInstance()
{
  return motorController;
}

CameraCapture& Robot::getCameraCaptureInstance()
{
  return cameraCapture;
}

// ColorSensor& Robot::getColorSensorInstance()
// {
//   return colorSensor;
// }
