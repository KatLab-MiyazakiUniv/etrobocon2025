/**
 * @file   MiniFigCameraAction.cpp
 * @brief  配置エリアAにおけるミニフィグ撮影動作
 * @author nishijima515
 */

#include "MiniFigCameraAction.h"

using namespace std;

MiniFigCameraAction::MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                                         int _postTargetAngle, double _targetRotationSpeed,
                                         double _backTargetDistance, double _forwardTargetDistance,
                                         double _backSpeed, double _forwardSpeed, int position)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    targetRotationSpeed(_targetRotationSpeed),
    backTargetDistance(_backTargetDistance),
    forwardTargetDistance(_forwardTargetDistance),
    backSpeed(_backSpeed),
    forwardSpeed(_forwardSpeed),
    position(position)
{
}

bool MiniFigCameraAction::isMetPreCondition()
{
  if(position != 0 && robot.getMiniFigDirectionResult().wasDetected
     && robot.getMiniFigDirectionResult().direction != static_cast<MiniFigDirection>(position)) {
    printf("ミニフィグの撮影動作は行わない。\n");
    return false;
  } else {
    return true;
  }
}

void MiniFigCameraAction::run()
{
  // 事前準備
  isMetPreCondition();

  // 撮影のための回頭をする
  AngleRotation preAR(robot, preTargetAngle, targetRotationSpeed, isClockwise);
  preAR.run();

  // 動作安定のためのスリープ
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // 後退
  // DistanceStraight(Robot& _robot, double _targetDistance, double _speed);
  DistanceStraight back(robot, backTargetDistance, -backSpeed);
  back.run();

  // 判定用の写真を撮影
  cv::Mat frame;
  robot.getCameraCaptureInstance().getFrame(frame);

  if(position == 0) {
    // 向きの判定とresultの更新(detection)は一回目(初期位置での)の撮影でしか行わない
    std::cout << "判定動作実施" << std::endl;
    MiniFigDirectionDetection detection(robot, frame);
    detection.run();
    std::cout << "判定動作終了" << std::endl;

    if(robot.getMiniFigDirectionResult().wasDetected
       && robot.getMiniFigDirectionResult().direction == MiniFigDirection::FRONT) {
      // FRONT方向の画像を保存
      FrameSave::frameSave(frame, filePath, "upload_front_fig.jpg");
    }

  } else if(position != 0 && robot.getMiniFigDirectionResult().wasDetected
            && robot.getMiniFigDirectionResult().direction
                   == static_cast<MiniFigDirection>(position)) {
    // 一回目の撮影でミニフィグが検出されていて、向きがFRONTじゃなければ、二回目の撮影でのミニフィグの向きは確実にFRONTになる。
    std::cout << "正面での撮影" << std::endl;
    FrameSave::frameSave(frame, filePath, "upload_front_fig");
  } else if(position != 0 && !robot.getMiniFigDirectionResult().wasDetected) {
    // 一回目検出falseなら、残り、3回の撮影は確定する。
    // 一回目の撮影でミニフィグが検出されていない場合は、残り3つのすべてのpositionで撮影を行い、画像をpositionごとに保存する。
    std::cout << "ミニフィグ向き判定用写真の撮影" << std::endl;
    FrameSave::frameSave(frame, filePath, "Fig_" + to_string(position));
  }

  // 動作安定のためのスリープ
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // 前進
  DistanceStraight forward(robot, forwardTargetDistance, forwardSpeed);
  forward.run();

  // 黒線復帰のための回頭をする
  AngleRotation postAR(robot, postTargetAngle, targetRotationSpeed, !isClockwise);
  postAR.run();
}