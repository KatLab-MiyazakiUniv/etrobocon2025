/**
 * @file   Robot.cpp
 * @brief  外部リソースのインスタンスを管理するクラス
 * @author takuchi17
 */

#include "Robot.h"

Robot::Robot() : motorController(), cameraCapture() /*, colorSensor(EPort::PORT_E)*/ {}

MotorController& Robot::getMotorControllerInstance()
{
  return motorController;
}

CameraCapture& Robot::getCameraCaptureInstance()
{
  return cameraCapture;
}

// spikeapi::ColorSensor& Robot::getColorSensorInstance()
// {
//   return colorSensor;
// }
