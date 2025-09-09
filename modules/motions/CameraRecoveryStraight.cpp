/**
 * @file   CameraRecoveryStraight.cpp
 * @brief  カメラ検出失敗時の復帰動作クラス
 * @author nishijima515
 */

#include "CameraRecoveryStraight.h"

CameraRecoveryStraight::CameraRecoveryStraight(
    Robot& _robot, double _straightDistance, double _targetDistance, double _speed,
    std::unique_ptr<BoundingBoxDetector> _boundingBoxDetector)
  : CompositeMotion(_robot),
    straightDistance(_straightDistance),
    targetDistance(_targetDistance),
    speed(_speed),
    boundingBoxDetector(std::move(_boundingBoxDetector))
{
}

void CameraRecoveryStraight::run()
{
  cv::Mat frame;

  double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
  double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
  double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  double runDistance = 0.0;

  while(1) {

  // 初期検出確認
    std::this_thread::sleep_for(
        std::chrono::milliseconds(33));  // フレームを確実に取得するためのスリープ
    
    if(!robot.getCameraCaptureInstance().getFrame(frame)) {
      std::cout << "フレーム取得失敗のため終了\n" << std::endl;
      return;
    }

  // 複数フレームから現在の写真を1枚取得する
    for(int i = 0; i < FRAME_NUMBER; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(33));  // フレーム間の待機(33ミリ秒)
      robot.getCameraCaptureInstance().getFrame(frame);
    }

    boundingBoxDetector->detect(frame, result);

  // 既に検出できた場合、復帰動作を行わない
    if(result.wasDetected) {
      std::cout << "復帰動作は行わない\n" << std::endl;
      return;
    }

    double initialRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
    double initialLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);
    // 指定された距離・スピードで直進復帰
    DistanceStraight straight(robot, straightDistance, speed);
    straight.run();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒待機

    // 複数フレームから現在の写真を1枚取得する
    for(int j = 0; j < FRAME_NUMBER; j++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(33));  // フレーム間の待機(33ミリ秒)
      robot.getCameraCaptureInstance().getFrame(frame);
    }

    boundingBoxDetector->detect(frame, result);

    double currentRightMotorCount = robot.getMotorControllerInstance().getRightMotorCount();
    double currentLeftMotorCount = robot.getMotorControllerInstance().getLeftMotorCount();
    double currentDistance = Mileage::calculateMileage(currentRightMotorCount, currentLeftMotorCount);

    runDistance += fabs(currentDistance - initialDistance);

    if(result.wasDetected) {
      std::cout << "復帰動作完了\n" << std::endl;
      std::cout << "走行距離: " << runDistance << " 目標距離: " << targetDistance << std::endl;
        // 検出成功
        break;
    }
    std::cout << "復帰動作失敗\n" << std::endl;
    std::cout << "走行距離: " << runDistance << " 目標距離: " << targetDistance << std::endl;
  }
  std::cout << "復帰動作距離オーバー\n" << std::endl;
  return;
}
