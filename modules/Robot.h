/**
 * @file   Robot.h
 * @brief  外部リソースのインスタンスを管理するクラス
 * @author takuchi17
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "spikeapi.h"
#include "MotorController.h"
#include "CameraCapture.h"
#include "ColorSensor.h"
#include "Clock.h"

class Robot {
 public:
  /**
   * コンストラクタ
   * @brief 外部リソースのインスタンスを初期化する
   */
  Robot();

  /**
   * @brief MotorControllerのインスタンスの参照を返す
   * @return メンバ変数motorController(MotorControllerのインスタンス)の参照
   */
  MotorController& getMotorControllerInstance();

  /**
   * @brief CameraCaptureのインスタンスの参照を返す
   * @return メンバ変数cameraCapture(CameraCaptureのインスタンス)の参照
   */
  CameraCapture& getCameraCaptureInstance();

  /**
   * @brief ColorSensorのインスタンスの参照を返す
   * @return メンバ変数colorSensor(ColorSensorのインスタンス)の参照
   */
  spikeapi::ColorSensor& getColorSensorInstance();
  spikeapi::Clock& getClockInstance();

 private:
  MotorController motorController;    // MotorControllerインスタンス
  CameraCapture cameraCapture;        // CameraCaptureインスタンス
  spikeapi::ColorSensor colorSensor;  // ColorSensorインスタンス
  spikeapi::Clock clock;
};

#endif
