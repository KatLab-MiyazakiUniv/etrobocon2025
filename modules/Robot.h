#ifndef ROBOT_H
#define ROBOT_H

#include "SpikeClient.h"
#include "MotorController.h"
#include "ColorSensorClient.h"
#include "ClockClient.h"
#include "ButtonClient.h"
#include "ForceSensorClient.h"
#include "DisplayClient.h"
#include "CameraCapture.h"
#include "MiniFigDirectionDetector.h"
#include "BackgroundDirectionDetector.h"

class Robot {
 public:
  Robot();
  Robot(ICameraCapture& cam);

  MotorController& getMotorControllerInstance();
  ICameraCapture& getCameraCaptureInstance();
  ColorSensorClient& getColorSensorInstance();
  ClockClient& getClockInstance();
  ButtonClient& getButtonInstance();
  ForceSensorClient& getForceSensorInstance();
  DisplayClient& getDisplayInstance();
  MiniFigDirectionResult& getMiniFigDirectionResult();
  BackgroundDirectionResult& getBackgroundDirectionResult();
  void setIsLeftEdge(bool isLeft);
  bool getIsLeftEdge() const;

 private:
  SpikeClient spikeClient;
  MotorController motorController;
  CameraCapture defaultCameraCapture;
  ICameraCapture& cameraCapture;
  ColorSensorClient colorSensor;
  ClockClient clock;
  ButtonClient button;
  ForceSensorClient forceSensor;
  DisplayClient display;
  MiniFigDirectionResult miniFigDirectionResult;
  BackgroundDirectionResult backgroundDirectionResult;
  bool isLeftEdge = true;
};

#endif
