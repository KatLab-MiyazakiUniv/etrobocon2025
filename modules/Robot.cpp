/**
 * @file   Robot.cpp
 * @brief  ハードウェアとのインターフェースや攻略状況を管理するクラス
 * @author takuchi17
 */

#include "Robot.h"

Robot::Robot(SpikeClient& client)
  : spikeClient(client),
    motorController(spikeClient),
    defaultCameraCapture(),
    cameraCapture(defaultCameraCapture),
    colorSensor(spikeClient),
    clock(spikeClient),
    button(spikeClient),
    forceSensor(spikeClient),
    display(spikeClient),
    robotStateCache(spikeClient)  // Initialize RobotStateCache
{
}

Robot::Robot(SpikeClient& client, ICameraCapture& cam)
  : spikeClient(client),
    motorController(spikeClient),
    defaultCameraCapture(),
    cameraCapture(cam),
    colorSensor(spikeClient),
    clock(spikeClient),
    button(spikeClient),
    forceSensor(spikeClient),
    display(spikeClient),
    robotStateCache(spikeClient)  // Initialize RobotStateCache
{
}

MotorControllerClient& Robot::getMotorControllerInstance()
{
  return motorController;
}

ICameraCapture& Robot::getCameraCaptureInstance()
{
  return cameraCapture;
}

ColorSensorClient& Robot::getColorSensorInstance()
{
  return colorSensor;
}

ClockClient& Robot::getClockInstance()
{
  return clock;
}

ButtonClient& Robot::getButtonInstance()
{
  return button;
}

ForceSensorClient& Robot::getForceSensorInstance()
{
  return forceSensor;
}

DisplayClient& Robot::getDisplayInstance()
{
  return display;
}

MiniFigDirectionResult& Robot::getMiniFigDirectionResult()
{
  return miniFigDirectionResult;
}

BackgroundDirectionResult& Robot::getBackgroundDirectionResult()
{
  return backgroundDirectionResult;
}

void Robot::setIsLeftEdge(bool isLeft)
{
  isLeftEdge = isLeft;
}

bool Robot::getIsLeftEdge() const
{
  return isLeftEdge;
}

RobotStateCache& Robot::getRobotStateCacheInstance()
{
  return robotStateCache;
}
