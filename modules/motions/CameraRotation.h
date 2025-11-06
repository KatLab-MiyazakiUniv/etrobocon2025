// /**
//  * @file   CameraRotation.h
//  * @brief  カメラ画像を用いた中心補正回頭クラス
//  */

// #ifndef CAMERA_ROTATION_H
// #define CAMERA_ROTATION_H

// #include "CorrectingRotation.h"

// class CameraRotation : public CorrectingRotation {
//  public:
//   CameraRotation(Robot& _robot, int _targetXCoordinate, double tolerance = 5.0,
//                  const PidGain& pidGain,
//                  const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

//  protected:
//   void prepare() override;
//   bool isMetPreCondition() override;
//   bool isMetContinuationCondition(int detectedX) override;
//   void updateMotorControl(int detectedX) override;

//  private:
//   int _targetXCoordinate;                                            // 目標x座標（画像中央など）
//   const CameraServer::BoundingBoxDetectorRequest _detectionRequest;  // 検出リクエスト
//   double _tolerance;                                                 // 許容誤差(px)
//   double _xError;                                                    // 現在の誤差(px)
//   const PidGain _xPid;                                               // PID制御器
// };

// #endif
