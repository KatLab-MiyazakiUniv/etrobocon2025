/**
 * @file   GetCorrectionAngle.h
 * @brief  補正対象の色の矩形が中心に来るような補正角度を返すクラス
 * @author nishijima515
 */

#ifndef GET_CORRECTION_ANGLE_H
#define GET_CORRECTION_ANGLE_H

#include "Robot.h"
#include "SocketProtocol.h"
#include <cmath>
#include "Calibrator.h"

struct GetCorrectionAngleResult {
  bool isClockwise;        // 回頭方向 true:時計回り, false:反時計回り
  double correctionAngle;  // 補正角度
};

class GetCorrectionAngle {
 public:
  /**
   * @brief コンストラクタ
   * @param _robot ロボット本体への参照
   */
  GetCorrectionAngle(Robot& _robot);

  /**
   * @brief 補正対象の色の矩形が中心に来るような補正角度を返す
   * @param detectionRequest カメラサーバーへの検出リクエスト
   * @param targetXCoordinate 目標X座標
   * @return 補正角度[度]
   */
  GetCorrectionAngleResult GetCorrectAngle(
      int targetXCoordinate, const CameraServer::BoundingBoxDetectorRequest& detectionRequest);

 private:
  Robot& robot;
  BoundingBoxDetectionResult result;
  GetCorrectionAngleResult correctionResult;
};

#endif