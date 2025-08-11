#include "Robot.h"

Robot::Robot()
  : spikeClient(),
    motorController(spikeClient),
    defaultCameraCapture(),
    cameraCapture(defaultCameraCapture),
    colorSensor(spikeClient),
    clock(spikeClient),
    button(spikeClient),
    forceSensor(spikeClient),
    display(spikeClient)
{
}

Robot::Robot(ICameraCapture& cam)
  : spikeClient(),
    motorController(spikeClient),
    defaultCameraCapture(),
    cameraCapture(cam),
    colorSensor(spikeClient),
    clock(spikeClient),
    button(spikeClient),
    forceSensor(spikeClient),
    display(spikeClient)
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