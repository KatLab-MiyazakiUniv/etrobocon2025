#ifndef ROBOT_H
#define ROBOT_H

#include "SpikeClient.h"  // This will be the real SpikeClient for production
#include "MotorControllerClient.h"
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
  // New constructor for dependency injection of SpikeClient
  explicit Robot(SpikeClient& client);
  Robot(SpikeClient& client, ICameraCapture& cam);  // Updated constructor

  MotorControllerClient& getMotorControllerInstance();
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
  SpikeClient& spikeClient;  // Now a reference
  MotorControllerClient motorController;
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
