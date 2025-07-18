/**
 * @file   Robot.cpp
 * @brief  外部リソースのインスタンスを管理するクラス
 * @author takuchi17
 */

#include "Robot.h"

Robot::Robot()
  : motorController(),
    defaultCameraCapture(),
    cameraCapture(defaultCameraCapture),
    colorSensor(EPort::PORT_E),
    clock(),
    button(),
    forceSensor(EPort::PORT_D),
    display()
{
}

// DI(依存性注入)用コンストラクタ
Robot::Robot(ICameraCapture& cam)
  : motorController(),
    defaultCameraCapture(),
    cameraCapture(cam),
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

ICameraCapture& Robot::getCameraCaptureInstance()
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

spikeapi::Button& Robot::getButtonInstance()
{
  return button;
}

spikeapi::ForceSensor& Robot::getForceSensorInstance()
{
  return forceSensor;
}

spikeapi::Display& Robot::getDisplayInstance()
{
  return display;
}

MiniFigDirectionResult& Robot::getMiniFigDirectionResult()
{
  return miniFigDirectionResult;  // ミニフィグの向き検出結果を返す
}

void Robot::setIsLeftEdge(bool isLeft)
{
  isLeftEdge = isLeft;  // エッジの左右判定を設定
}

bool Robot::getIsLeftEdge() const
{
  return isLeftEdge;
}
