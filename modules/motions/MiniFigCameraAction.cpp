/**
 * @file   MiniFigCameraAction.cpp
 * @brief  配置エリアAにおけるミニフィグ撮影動作
 * @author nishijima515
 */

#include "MiniFigCameraAction.h"

using namespace std;

MiniFigCameraAction::MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                                         int _postTargetAngle, double _targetRotationSpeed,
                                         double _targetDistance, double _forwardSpeed,
                                         double _backSpeed, int position)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    targetRotationSpeed(_targetRotationSpeed),
    targetDistance(_targetDistance),
    forwardSpeed(_forwardSpeed),
    backSpeed(_backSpeed),
    position(position)
{
}

void MiniFigCameraAction::run()
{
  if(robot.getMiniFigDirectionResult().wasDetected
     && robot.getMiniFigDirectionResult().direction == MiniFigDirection::FRONT) {
    // ミニフィグの向きがFRONTの場合は、撮影動作を行わない
    printf("ミニフィグの向きがFRONTなので撮影動作は行いません。\n");
    return;
  }
  cv::Mat frame;
  MiniFigDirectionDetection detection(robot, frame);
  // figの向きがFRONTの場合はそれ以外の場合は、撮影動作を行う。

  AngleRotation preAR(robot, preTargetAngle, targetRotationSpeed, !isClockwise);
  AngleRotation postAR(robot, postTargetAngle, targetRotationSpeed, isClockwise);

  // 撮影のための回頭をする
  if(preTargetAngle != 0) {
    preAR.run();
  }

  // 後退
  // DistanceStraight(Robot& _robot, double _targetDistance, double _speed);
  DistanceStraight back(robot, targetDistance, backSpeed);
  back.run();

  // 撮影動作（撮影・判定・保存）を行う。
  robot.getCameraCaptureInstance().getFrame(frame);
  if(position == 0) {
    // 向きの判定とresultの更新(detection)は一回目(初期位置での)の撮影でしか行わない
    std::cout << "判定動作実施" << std::endl;
    detection.run();
    std::cout << "判定動作終了" << std::endl;
  } else if(position != 0 && robot.getMiniFigDirectionResult().wasDetected) {
    // 一回目の撮影でミニフィグが検出されていて、向きがFRONTじゃなければ、二回目の撮影でのミニフィグの向きは確実にFRONTになる。
    std::cout << "2回目検出あり" << std::endl;
    FrameSave::frameSave(frame, "etrobocon2025/datafiles/figures/", "upload_front_fig.jpeg");
  } else if(position != 0 && !robot.getMiniFigDirectionResult().wasDetected) {
    // 一回目検出falseなら、残り、3回の撮影は確定する。
    // 一回目の撮影でミニフィグが検出されていない場合は、残り3つのすべてのpositionで撮影を行い、画像をpositionごとに保存する。
    std::cout << "2回目検出なし" << std::endl;
    FrameSave::frameSave(frame, "etrobocon2025/datafiles/figures/",
                         "Fig_" + to_string(position) + ".jpeg");
  }

  // 前進
  DistanceStraight forward(robot, targetDistance, forwardSpeed);
  forward.run();

  // 黒線復帰のための回頭をする
  if(postTargetAngle != 0) {
    postAR.run();
  }
}