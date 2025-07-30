// /**
//  * @file   MiniFigCameraAction.cpp
//  * @brief  配置エリアAにおけるミニフィグ撮影動作
//  * @author nishijima515
//  */

// #include "MiniFigCameraAction.h"

// using namespace std;

// MiniFigCameraAction::MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
//                                          int _postTargetAngle, double _targetRotationSpeed,
//                                          double _backTargetDistance, double _forwardTargetDistance,
//                                          double _backSpeed, double _forwardSpeed, int _position)
//   : CompositeMotion(_robot),
//     isClockwise(_isClockwise),
//     preTargetAngle(_preTargetAngle),
//     postTargetAngle(_postTargetAngle),
//     targetRotationSpeed(_targetRotationSpeed),
//     backTargetDistance(_backTargetDistance),
//     forwardTargetDistance(_forwardTargetDistance),
//     backSpeed(_backSpeed),
//     forwardSpeed(_forwardSpeed),
//     position(_position)
// {
// }

// bool MiniFigCameraAction::isMetPreCondition()
// {
//   if(position != 0 && robot.getMiniFigDirectionResult().wasDetected
//      && robot.getMiniFigDirectionResult().direction != static_cast<MiniFigDirection>(position)) {
//     std::cout << "ミニフィグの撮影動作は行わない。" << std::endl;
//     return false;
//   } else {
//     return true;
//   }
// }

// // 判定動作を行う関数
// void MiniFigCameraAction::detectDirection(cv::Mat& frame)
// {
//   MiniFigDirectionDetector detector;
//   // ミニフィグの向きを判定
//   detector.detect(frame, robot.getMiniFigDirectionResult());

//   // 検出結果を取得
//   MiniFigDirectionResult& result = robot.getMiniFigDirectionResult();

//   // デバッグ出力
//   if(!result.wasDetected) {
//     std::cout << "ミニフィグが検出されませんでした" << std::endl;
//   }
//   switch(result.direction) {
//     case MiniFigDirection::FRONT:
//       std::cout << "ミニフィグの向き: FRONT" << std::endl;
//       break;
//     case MiniFigDirection::BACK:
//       std::cout << "ミニフィグの向き: BACK" << std::endl;
//       break;
//     case MiniFigDirection::LEFT:
//       std::cout << "ミニフィグの向き: LEFT" << std::endl;
//       break;
//     case MiniFigDirection::RIGHT:
//       std::cout << "ミニフィグの向き: RIGHT" << std::endl;
//       break;
//   }
// }

// void MiniFigCameraAction::run()
// {
//   // 事前条件判定
//   if(!isMetPreCondition()) {
//     return;
//   }

//   // 撮影のための回頭をする
//   AngleRotation preAR(robot, preTargetAngle, targetRotationSpeed, isClockwise);
//   preAR.run();

//   // 動作安定のためのスリープ
//   std::this_thread::sleep_for(std::chrono::milliseconds(10));

//   // 後退
//   DistanceStraight back(robot, backTargetDistance, -backSpeed);
//   back.run();

//   // 判定用の写真を撮影
//   cv::Mat frame;
//   for(int i = 0; i < 5; i++) {
//     robot.getCameraCaptureInstance().getFrame(frame);
//   }

//   if(position == 0) {
//     // 向きの判定とresultの更新(detection)は一回目(初期位置での)の撮影でしか行わない
//     std::cout << "判定動作実施" << std::endl;
//     detectDirection(frame);
//     std::cout << "判定動作終了" << std::endl;

//     if(robot.getMiniFigDirectionResult().wasDetected
//        && robot.getMiniFigDirectionResult().direction == MiniFigDirection::FRONT) {
//       // FRONT方向の画像を保存
//       FrameSave::save(frame, filePath, uploadFileName);
//     }

//   } else if(robot.getMiniFigDirectionResult().wasDetected) {
//     // 一回目の撮影でミニフィグが検出されていて、向きがFRONTじゃなければ、二回目の撮影でのミニフィグの向きは確実にFRONTになる。
//     std::cout << "正面での撮影" << std::endl;
//     FrameSave::save(frame, filePath, uploadFileName);
//   } else {
//     // 一回目検出falseなら、残り、3回の撮影は確定する。
//     // 一回目の撮影でミニフィグが検出されていない場合は、残り3つのすべてのpositionで撮影を行い、画像をpositionごとに保存する。
//     std::cout << "ミニフィグ向き判定用写真の撮影" << std::endl;
//     FrameSave::save(frame, filePath, "Fig_" + to_string(position));
//   }

//   // 動作安定のためのスリープ
//   std::this_thread::sleep_for(std::chrono::milliseconds(10));

//   robot.getMotorControllerInstance().setLeftMotorPower(0);
//   robot.getMotorControllerInstance().setRightMotorPower(0);
//   // 前進
//   DistanceStraight forward(robot, forwardTargetDistance, forwardSpeed);
//   forward.run();

//   // 動作安定のためのスリープ
//   std::this_thread::sleep_for(std::chrono::milliseconds(10));

//   robot.getMotorControllerInstance().setLeftMotorPower(0);
//   robot.getMotorControllerInstance().setRightMotorPower(0);

//   // 動作安定のためのスリープ
//   std::this_thread::sleep_for(std::chrono::milliseconds(10));

//   // 黒線復帰のための回頭をする
//   AngleRotation postAR(robot, postTargetAngle, targetRotationSpeed, !isClockwise);
//   postAR.run();
// }