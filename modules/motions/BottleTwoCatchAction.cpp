/**
 * @file   BottleTwoCatchAction.cpp
 * @brief  ボトル2つ目のキャッチ動作クラス
 * @author nishijima515
 */

#include "BottleTwoCatchAction.h"

// コンストラクタ
BottleTwoCatchAction::BottleTwoCatchAction(
    Robot& _robot, double _forwardDistance, double _idsSpeed, const PidGain& _idsPidGain,
    double _ultrasonicDistance, double _udclSpeed, double _angle, double _rotatePower,
    const PidGain& _udclPidGain, const CameraServer::BoundingBoxDetectorRequest& _detectionRequest,
    int _roiShrinkValue, double _minimumDistance, double _pcidsForwardDistance, int _maxSwingCount)
  : CompositeMotion(_robot),
    forwardDistance(_forwardDistance),
    pcidsForwardDistance(_pcidsForwardDistance),
    idsSpeed(_idsSpeed),
    idsPidGain(_idsPidGain),
    ultrasonicDistance(_ultrasonicDistance),
    udclSpeed(_udclSpeed),
    angle(_angle),
    rotatePower(_rotatePower),
    udclPidGain(_udclPidGain),
    detectionRequest(_detectionRequest),
    roiShrinkValue(_roiShrinkValue),
    minimumDistance(_minimumDistance),
    maxSwingCount(_maxSwingCount)
{
}

void BottleTwoCatchAction::run()
{
  // ボトル探索開始時点の走行距離を取得する
  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // PCIDS用にROIを狭める
  CameraServer::BoundingBoxDetectorRequest pcidsDetectionRequest = detectionRequest;
  pcidsDetectionRequest.roi.x += roiShrinkValue;
  pcidsDetectionRequest.roi.y += roiShrinkValue;
  pcidsDetectionRequest.roi.width -= roiShrinkValue * 2;
  pcidsDetectionRequest.roi.height -= roiShrinkValue * 2;

  // ボトル探索を検出する、もしくは指定の距離で止まるまで前進
  PictureColorDistanceStraight pcids(robot, pcidsForwardDistance, idsSpeed, idsPidGain,
                                     pcidsDetectionRequest, minimumDistance);
  pcids.run();

  // 検出失敗時の首振り制御用の変数
  bool isSwingClockwise = false;  // 回頭方向（左奥が見つけられないことが多いので左奥から）
  int swingFlipCount = 1;         // 方向を反転す回頭回数（1, 3, 6, 10, 15, …）
  int currentSwingCount = 0;

  while(true) {
    // 最新フレームに更新するためにスナップショットを連続で取得
    CameraServer::SnapshotActionRequest request{};
    request.command = CameraServer::Command::TAKE_SNAPSHOT;
    std::strncpy(request.fileName, "warmup", sizeof(request.fileName));
    for(int i = 0; i < 5; ++i) {
      CameraServer::SnapshotActionResponse response{};
      robot.getSocketClient().executeSnapshotAction(request, response);
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // カメラでボトルを検出
    CameraServer::BoundingBoxDetectorResponse response;
    bool success = robot.getSocketClient().executeLineDetection(detectionRequest, response);

    // 通信成功かつボトルが検出ができた場合
    if(success && response.result.wasDetected) {
      UltrasonicDistanceCameraLineTrace udcl(robot, ultrasonicDistance, forwardDistance, udclSpeed,
                                             400, udclPidGain, detectionRequest);
      udcl.run();
      break;
    } else {
      // ボトルを探すために回頭（1回ごとに検出処理へ戻る）
      IMUAngleRotation searchRotation(robot, 10, rotatePower, isSwingClockwise,
                                      PidGain(0.036, 0.012, 0.03), false);
      searchRotation.run();

      // 動作安定のために多めに500ミリ秒待機
      std::this_thread::sleep_for(std::chrono::milliseconds(500));

      currentSwingCount++;

      // 回頭回数が上限に達したら終了
      if(currentSwingCount >= maxSwingCount) break;

      // 三角数列（1, 3, 6, 10, 15, …）の時に反転
      if(swingFlipCount == currentSwingCount) {
        swingFlipCount
            = currentSwingCount + (static_cast<int>(std::sqrt(1 + 8 * currentSwingCount)) + 1) / 2;
        isSwingClockwise = !isSwingClockwise;
      }
    }
  }

  // 動作終了時点の走行距離を取得する
  double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

  double totalAngleToTurn = robot.getIMUControllerInstance().getAngle();

  double runDistance = fabs(currentDistance - initialDistance);
  double widthMoved = runDistance * sin(totalAngleToTurn * PI / 180.0);

  std::cout << "走った距離: " << runDistance << "SCA開始時点からの傾き：" << totalAngleToTurn
            << std::endl;
  std::cout << "sin：" << sin(totalAngleToTurn) << std::endl;
  std::cout << "横方向のズレ：" << widthMoved << std::endl;

  // 回頭
  IMUMinAngleRotation imumar(robot, angle, rotatePower, PidGain(0.036, 0.012, 0.03));
  imumar.run();

  // 最大直進距離を設定
  double targetStraightDistance = forwardDistance;  // forwardDistanceは1100mm程度に設定される想定
  // 補正距離を算出
  double nextDistance = targetStraightDistance - runDistance;
  if(nextDistance < 0) {
    nextDistance = 0;
  }

  // 距離補正のための直進
  IMUDistanceStraight nextids(robot, nextDistance, idsSpeed, idsPidGain);
  nextids.run();
}
