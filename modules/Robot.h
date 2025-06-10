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
#include "Button.h"
#include "ForceSensor.h"
#include "Display.h"

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

  /**
   * @brief Clockのインスタンスの参照を返す
   * @return メンバ変数clock(Clockのインスタンス)の参照
   */
  spikeapi::Clock& getClockInstance();

  /**
   * @brief Buttonのインスタンスの参照を返す
   * @return メンバ変数button(Buttonのインスタンス)の参照
   */
  spikeapi::Button& getButtonInstance();

  /**
   * @brief ForceSensorのインスタンスの参照を返す
   * @return メンバ変数forceSensor(ForceSensorのインスタンス)の参照
   */
  spikeapi::ForceSensor& getForceSensorInstance();

  /**
   * @brief Displayのインスタンスの参照を返す
   * @return メンバ変数display(Displayのインスタンス)の参照
   */
  spikeapi::Display& getDisplayInstance();

  /**
   * @brief エッジの左右判定を設定する
   * @param isLeft true:左エッジ, false:右エッジ
   */
  void setIsLeftEdge(bool isLeft);

  /**
   * @brief エッジの左右判定を取得する
   * @return true:左エッジ, false:右エッジ
   */
  bool getIsLeftEdge() const;

 private:
  MotorController motorController;    // MotorControllerインスタンス
  CameraCapture cameraCapture;        // CameraCaptureインスタンス
  spikeapi::ColorSensor colorSensor;  // ColorSensorインスタンス
  spikeapi::Clock clock;              // Clockインスタンス
  spikeapi::Button button;            // Buttonインスタンス
  spikeapi::ForceSensor forceSensor;  // ForceSensorインスタンス
  spikeapi::Display display;          // Displayインスタンス
  // formatチェックをパスするためのコメント
  bool isLeftEdge = true;  // 左エッジを走行するかの真偽値
                           // （true: 左エッジ、false: 右エッジ）、初期値は左エッジ
};

#endif