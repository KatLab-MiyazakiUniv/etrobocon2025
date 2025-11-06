// /**
//  * @file   CorrectingRotation.cpp
//  * @brief  補正回頭動作の親クラス
//  * @author nishijima515
//  */

// #include "CorrectingRotation.h"

// CorrectingRotation::CorrectingRotation(
//     Robot& _robot, int _targetXCoordinate, const PidGain& _pidGain,
//     const CameraServer::BoundingBoxDetectorRequest& _detectionRequest)
//   : Motion(_robot),
//     targetXCoordinate(_targetXCoordinate),
//     pidGain(_pidGain),
//     detectionRequest(_detectionRequest)
// {
// }

// void CorrectingRotation::run()
// {
//   Pid pid(pidGain.kp, pidGain.ki, pidGain.kd, targetXCoordinate);
//   // 事前条件を判定する
//   if(!isMetPreCondition()) {
//     return;
//   }

//   // 事前準備
//   prepare();

//   SocketClient& client = robot.getSocketClient();

//   MotorController& motorController = robot.getMotorControllerInstance();

//   if(!isMetPreCondition()) return;
//   prepare();

//   // 継続条件を満たしている間ループ
//   while(isMetContinuationCondition()) {
//     // ライン検出をサーバーに依頼
//     CameraServer::BoundingBoxDetectorResponse response;
//     bool success = client.executeLineDetection(detectionRequest, response);

//     // 通信失敗、または検出できなかった場合
//     if(!success || !response.result.wasDetected) {
//       continue;
//     }

//     // バウンディングボックスの中心X座標を計算
//     double currentX = (response.result.topLeft.x + response.result.bottomRight.x) / 2.0;
//     // 動的モーター制御
//     updateMotorControl();
//   }

//   // モーターを停止
//   motorController.stopWheelsMotor();
//   motorController.resetWheelsMotorPower();
// }
