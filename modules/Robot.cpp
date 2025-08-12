#include "Robot.h"

// Constructor for dependency injection of SpikeClient
Robot::Robot(SpikeClient& client)
  : spikeClient(client),  // Initialize reference
    motorController(spikeClient),
    defaultCameraCapture(),
    cameraCapture(defaultCameraCapture),  // Use defaultCameraCapture
    colorSensor(spikeClient),
    clock(spikeClient),
    button(spikeClient),
    forceSensor(spikeClient),
    display(spikeClient)
{
}

// Constructor with camera injection
Robot::Robot(SpikeClient& client, ICameraCapture& cam)
  : spikeClient(client),  // Initialize reference
    motorController(spikeClient),
    defaultCameraCapture(),
    cameraCapture(cam),  // Use injected camera
    colorSensor(spikeClient),
    clock(spikeClient),
    button(spikeClient),
    forceSensor(spikeClient),
    display(spikeClient)
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
