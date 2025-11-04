// /**
//  * @file   CameraRotation.cpp
//  * @brief  カメラ画像を用いた中心補正回頭クラス
//  */

// #include "CameraRotation.h"

// CameraRotation::CameraRotation(Robot& _robot, int _targetXCoordinate, const PidGain& _pidGain,
//                                const CameraServer::BoundingBoxDetectorRequest& _detectionRequest,
//                                double _tolerance)
//   : CorrectingRotation(_robot, _targetXCoordinate, _pidGain, _detectionRequest),
//     _targetXCoordinate(_targetXCoordinate),
//     _tolerance(_tolerance),
//     _xError(0.0),
//     _xPid(_pidGain.kp, _pidGain.ki, _pidGain.kd, 0.0)
// {
// }

// void CameraRotation::prepare()
// {
//   std::cout << "Camera rotation started. Target X = " << targetX << std::endl;
// }

// bool CameraRotation::isMetPreCondition()
// {
//   // 必要ならここにチェック追加（例：カメラ初期化済みか等）
//   return true;
// }

// bool CameraRotation::isMetContinuationCondition(int detectedX)
// {
//   xError = static_cast<double>(targetX - detectedX);
//   return std::abs(xError) > tolerance;
// }

// void CameraRotation::updateMotorControl(int detectedX)
// {
//   xError = static_cast<double>(targetX - detectedX);

//   // PID制御による補正値算出
//   double pidCorrection = xPid.calculatePid(xError, 0.01);

//   // x方向誤差が正 → 左回頭（右モータ強め）、負 → 右回頭（左モータ強め）
//   double leftPower = basePower - pidCorrection;
//   double rightPower = basePower + pidCorrection;

//   robot.getMotorControllerInstance().setLeftMotorPower(leftPower);
//   robot.getMotorControllerInstance().setRightMotorPower(rightPower);
// }
