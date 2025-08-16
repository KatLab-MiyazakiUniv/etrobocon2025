/**
 * @file   Robot.cpp
 * @brief  外部リソースのインスタンスを管理するクラス
 * @author takuchi17
 */

#include "Robot.h"

Robot::Robot()
  : motorController(),
    socketClient(),
    colorSensor(EPort::PORT_E),
    clock(),
    button(),
    forceSensor(EPort::PORT_D),
    display(),
    miniFigDirectionResult(),
    backgroundDirectionResult()
{
    socketClient.connectToServer();
}

Robot::~Robot()
{
    socketClient.disconnectFromServer();
}

MotorController& Robot::getMotorControllerInstance()
{
  return motorController;
}

SocketClient& Robot::getSocketClient()
{
  return socketClient;
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
  return miniFigDirectionResult;
}

BackgroundDirectionResult& Robot::getBackgroundDirectionResult()
{
  return backgroundDirectionResult;  // 背景の向き検出結果を返す
}

void Robot::setIsLeftEdge(bool isLeft)
{
  isLeftEdge = isLeft;  // エッジの左右判定を設定
}

bool Robot::getIsLeftEdge() const
{
  return isLeftEdge;
}
