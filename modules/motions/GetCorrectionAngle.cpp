/**
 * @file   GetCorrectionAngle.h
 * @brief  補正対象の色の矩形が中心に来るような補正角度を返すクラス
 * @author nishijima515
 */

#include "GetCorrectionAngle.h"

GetCorrectionAngle::GetCorrectionAngle(Robot& _robot) : robot(_robot) {}

GetCorrectionAngleResult GetCorrectionAngle::GetCorrectAngle(
    int targetXCoordinate, const CameraServer::BoundingBoxDetectorRequest& detectionRequest)
{
  // バウンディングボックスの中心X座標を計算
  // ライン検出をサーバーに依頼
  SocketClient& client = robot.getSocketClient();

  CameraServer::BoundingBoxDetectorResponse response;
  bool success = client.executeLineDetection(detectionRequest, response);

  // 通信失敗、または検出できなかった場合
  if(!success || !response.result.wasDetected) {
    correctionResult.isClockwise = false;
    correctionResult.correctionAngle = 0.0;
    return correctionResult;
  }

  // バウンディングボックスの中心X座標を計算
  double currentX = (response.result.topLeft.x + response.result.bottomRight.x) / 2.0;

  Calibrator isleftCorse(robot);
  // // 回頭方向を計算
  // if(currentX >= targetXCoordinate && isleftCorse.getIsLeftCourse() == true) {
  //   correctionResult.isClockwise = true;
  // } else if(currentX >= targetXCoordinate && isleftCorse.getIsLeftCourse() == false) {
  //   correctionResult.isClockwise = false;
  // } else if(currentX < targetXCoordinate && isleftCorse.getIsLeftCourse() == true) {
  //   correctionResult.isClockwise = false;
  // } else {
  //   correctionResult.isClockwise = true;
  // }

  if(currentX >= targetXCoordinate) {
    correctionResult.isClockwise = true;
  } else if(currentX < targetXCoordinate) {
    correctionResult.isClockwise = false;
  }

  // 補正角度を計算
  correctionResult.correctionAngle
      = atan2(currentX - targetXCoordinate, targetXCoordinate) * 180.0 / M_PI;

  if(correctionResult.correctionAngle < 0) {
    correctionResult.correctionAngle = correctionResult.correctionAngle * (-1);
  }

  return correctionResult;
}
