/**
 * @file   BackgroundCameraAction.cpp
 * @brief  風景撮影動作クラス
 * @author miyahara046
 */

#include "BackgroundCameraAction.h"

using namespace std;

BackgroundCameraAction::BackgroundCameraAction(Robot& _robot, bool _isClockwise,
                                               int _preTargetAngle, int _postTargetAngle,
                                               double _targetRotationSpeed, double _threshold,
                                               double _minArea, int _roiX, int _roiY, int _roiWidth,
                                               int _roiHeight, int _position)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    targetRotationSpeed(_targetRotationSpeed),
    threshold(_threshold),
    minArea(_minArea),
    roiX(_roiX),
    roiY(_roiY),
    roiWidth(_roiWidth),
    roiHeight(_roiHeight),
    position(_position)
{
}

bool BackgroundCameraAction::isMetPreCondition()
{
  if(position != 0 && robot.getBackgroundDirectionResult().wasDetected
     && robot.getBackgroundDirectionResult().direction
            != static_cast<BackgroundDirection>(position)) {
    std::cout << "風景の撮影動作は行わない。" << std::endl;
    return false;
  } else {
    return true;
  }
}

// 判定動作を行う関数
void BackgroundCameraAction::detectDirection(cv::Mat& frame)
{
  BackgroundDirectionDetector detector;
  // 風景の向きを判定
  detector.detect(frame, robot.getBackgroundDirectionResult());

  // 検出結果を取得
  BackgroundDirectionResult& result = robot.getBackgroundDirectionResult();

  // デバッグ出力
  if(!result.wasDetected) {
    std::cout << "風景が検出されませんでした" << std::endl;
  }
  switch(result.direction) {
    case BackgroundDirection::FRONT:
      std::cout << "風景の向き: FRONT" << std::endl;
      break;
    case BackgroundDirection::BACK:
      std::cout << "風景の向き: BACK" << std::endl;
      break;
    case BackgroundDirection::LEFT:
      std::cout << "風景の向き: LEFT" << std::endl;
      break;
    case BackgroundDirection::RIGHT:
      std::cout << "風景の向き: RIGHT" << std::endl;
      break;
  }
}

void BackgroundCameraAction::run()
{
  if(!isMetPreCondition()) return;

  // 撮影のため回頭
  AngleRotation preRotation(robot, preTargetAngle, targetRotationSpeed, isClockwise);
  preRotation.run();

  // 動作安定のためのスリープ
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  PlaCameraAction plaAction(robot, threshold, minArea, roiX, roiY, roiWidth, roiHeight);

  cv::Mat frame;

  // 判定用のフレームの獲得
  for(int i = 0; i < 5; ++i) {
    robot.getCameraCaptureInstance().getFrame(frame);
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }
  if(position == 0) {
    // 向きの判定とresultの更新(detection)は一回目(初期位置での)の撮影でしか行わない
    std::cout << "判定動作実施" << std::endl;
    detectDirection(frame);
    std::cout << "判定動作終了" << std::endl;

    if(robot.getBackgroundDirectionResult().wasDetected
       && robot.getBackgroundDirectionResult().direction == BackgroundDirection::FRONT) {
      // FRONT方向の画像を保存
      plaAction.run();
    }

  } else if(robot.getBackgroundDirectionResult().wasDetected) {
    // 一回目の撮影で風景が検出されていて、向きがFRONTじゃなければ、二回目の撮影での風景の向きは確実にFRONTになる。
    std::cout << "正面での撮影" << std::endl;
    plaAction.run();
  } else {
    // 一回目検出falseなら、残り、3回の撮影は確定する。
    // 一回目の撮影で風景が検出されていない場合は、残り3つのすべてのpositionで撮影を行い、画像をpositionごとに保存する。
    std::cout << "風景向き判定用写真の撮影" << std::endl;
    plaAction.setUploadName("Fig_" + to_string(position));
    plaAction.run();
  }

  // 動作安定のためのスリープ
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // 黒線復帰のための回頭をする
  AngleRotation postRotation(robot, postTargetAngle, targetRotationSpeed, !isClockwise);
  postRotation.run();
}