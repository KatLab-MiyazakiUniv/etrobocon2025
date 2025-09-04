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

// ミニフィグの向きを表す列挙体
enum class MiniFigDirection { FRONT, RIGHT, BACK, LEFT };

struct MiniFigDirectionResult {
  bool wasDetected = false;    // 検出が成功したかどうか
  MiniFigDirection direction;  // ミニフィグの向きを表す列挙体
};

// 風景の向きを表す列挙体
enum class BackgroundDirection { FRONT, RIGHT, BACK, LEFT };

// 結果格納用の構造体
struct BackgroundDirectionResult {
  bool wasDetected = false;       // 検出が成功したかどうか
  BackgroundDirection direction;  // 風景の向きを表す列挙体
};

class Robot {
 public:
  /**
   * コンストラクタ
   * @brief 外部リソースのインスタンスを初期化する
   */
  Robot();

  // DI用（カメラを差し替えたいテストで使用）
  Robot(ICameraCapture& cam);

  /**
   * @brief MotorControllerのインスタンスの参照を返す
   * @return メンバ変数motorController(MotorControllerのインスタンス)の参照
   */
  MotorController& getMotorControllerInstance();

  /**
   * @brief ICameraCaptureのインスタンスの参照を返す
   * @return メンバ変数cameraCapture(ICameraCaptureの実装インスタンス)への参照
   */
  ICameraCapture& getCameraCaptureInstance();

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
   * @brief ミニフィグの向き検出結果を取得する
   * @return ミニフィグの向き検出結果の参照
   */
  MiniFigDirectionResult& getMiniFigDirectionResult();

  /**
   * @brief 風景の向き検出結果を取得する
   * @return 風景の向き検出結果の参照
   */
  BackgroundDirectionResult& getBackgroundDirectionResult();
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
  MotorController motorController;                // MotorControllerインスタンス
  CameraCapture defaultCameraCapture;             // 実機用のCameraCaptureインスタンス
  ICameraCapture& cameraCapture;                  // 実際に使うカメラ（参照）
  spikeapi::ColorSensor colorSensor;              // ColorSensorインスタンス
  spikeapi::Clock clock;                          // Clockインスタンス
  spikeapi::Button button;                        // Buttonインスタンス
  spikeapi::ForceSensor forceSensor;              // ForceSensorインスタンス
  spikeapi::Display display;                      // Displayインスタンス
  MiniFigDirectionResult miniFigDirectionResult;  // ミニフィグの向き検出結果
  BackgroundDirectionResult backgroundDirectionResult;  // 風景の向き検出結果
  // formatチェックをパスするためのコメント
  bool isLeftEdge = true;  // 左エッジを走行するかの真偽値
                           // （true: 左エッジ、false: 右エッジ）、初期値は左エッジ
};

#endif