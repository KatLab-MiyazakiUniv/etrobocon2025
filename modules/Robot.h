/**
 * @file   Robot.h
 * @brief  ハードウェアとのインターフェースや攻略状況を管理するクラス
 * @author takuchi17
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "SpikeClient.h"
#include "MotorControllerClient.h"
#include "ColorSensorClient.h"
#include "ClockClient.h"
#include "ButtonClient.h"
#include "ForceSensorClient.h"
#include "DisplayClient.h"
#include "CameraCapture.h"
#include "MiniFigDirectionDetector.h"
#include "BackgroundDirectionDetector.h"
#include "RobotStateCache.h" // New include

class Robot {
 public:
  /**
   * @brief SpikeClientの依存性注入のためのコンストラクタ
   * @param client SpikeClientの参照
   */
  explicit Robot(SpikeClient& client);
  /**
   * @brief カメラ注入のためのコンストラクタ
   * @param client SpikeClientの参照
   * @param cam ICameraCaptureの参照
   */
  Robot(SpikeClient& client, ICameraCapture& cam);  // Updated constructor

  /**
   * @brief MotorControllerClientインスタンスを取得する
   * @return MotorControllerClientインスタンスの参照
   */
  MotorControllerClient& getMotorControllerInstance();
  /**
   * @brief ICameraCaptureインスタンスを取得する
   * @return ICameraCaptureインスタンスの参照
   */
  ICameraCapture& getCameraCaptureInstance();
  /**
   * @brief ColorSensorClientインスタンスを取得する
   * @return ColorSensorClientインスタンスの参照
   */
  ColorSensorClient& getColorSensorInstance();
  /**
   * @brief ClockClientインスタンスを取得する
   * @return ClockClientインスタンスの参照
   */
  ClockClient& getClockInstance();
  /**
   * @brief ButtonClientインスタンスを取得する
   * @return ButtonClientインスタンスの参照
   */
  ButtonClient& getButtonInstance();
  /**
   * @brief ForceSensorClientインスタンスを取得する
   * @return ForceSensorClientインスタンスの参照
   */
  ForceSensorClient& getForceSensorInstance();
  /**
   * @brief DisplayClientインスタンスを取得する
   * @return DisplayClientインスタンスの参照
   */
  DisplayClient& getDisplayInstance();
  /**
   * @brief MiniFigDirectionResultを取得する
   * @return MiniFigDirectionResultの参照
   */
  MiniFigDirectionResult& getMiniFigDirectionResult();
  /**
   * @brief BackgroundDirectionResultを取得する
   * @return BackgroundDirectionResultの参照
   */
  BackgroundDirectionResult& getBackgroundDirectionResult();

  /**
   * @brief RobotStateCacheインスタンスを取得する
   * @return RobotStateCacheインスタンスの参照
   */
  RobotStateCache& getRobotStateCacheInstance();
  /**
   * @brief isLeftEdgeを設定する
   * @param isLeft 左エッジの場合true
   */
  void setIsLeftEdge(bool isLeft);
  /**
   * @brief isLeftEdgeを取得する
   * @return 左エッジの場合true
   */
  bool getIsLeftEdge() const;

 private:
  SpikeClient& spikeClient;               // SpikeClientの参照
  MotorControllerClient motorController;  // モーターコントローラー
  CameraCapture defaultCameraCapture;     // デフォルトのカメラキャプチャ
  ICameraCapture& cameraCapture;          // カメラキャプチャインターフェース
  ColorSensorClient colorSensor;          // カラーセンサー
  ClockClient clock;                      // クロック
  ButtonClient button;                    // ボタン
  ForceSensorClient forceSensor;          // フォースセンサー
  DisplayClient display;                  // ディスプレイ
  MiniFigDirectionResult miniFigDirectionResult;        // ミニフィグの向き検出結果
  BackgroundDirectionResult backgroundDirectionResult;  // 背景の向き検出結果
  bool isLeftEdge = true;                               // 左エッジフラグ
  RobotStateCache robotStateCache;                      // ロボット状態キャッシュ
};

#endif
