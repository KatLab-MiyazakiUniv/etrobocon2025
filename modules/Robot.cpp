/**
 * @file   Robot.cpp
 * @brief  外部リソースのインスタンスを管理するクラス
 * @author takuchi17
 */

#include "Robot.h"

Robot::Robot()
  : motorController(),
    cameraCapture(),
    colorSensor(EPort::PORT_E),
    clock(),
    button(),
    forceSensor(EPort::PORT_D),
    display()
{
}

MotorController& Robot::getMotorControllerInstance()
{
  return motorController;
}

CameraCapture& Robot::getCameraCaptureInstance()
{
  return cameraCapture;
}

spikeapi::ColorSensor& Robot::getColorSensorInstance()
{
  return colorSensor;
}

spikeapi::Clock& Robot::getClockInstance()
{
  return clock;
}

void Robot::setIsLeftEdge(bool isLeft)
{
  isLeftEdge = isLeft;  // エッジの左右判定を設定
}

bool Robot::getIsLeftEdge() const
{
  return isLeftEdge;
}
