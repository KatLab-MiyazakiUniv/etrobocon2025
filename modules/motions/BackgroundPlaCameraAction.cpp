/**
 * @file   BackgroundPlaCameraAction.cpp
 * @brief  風景・プラレール撮影動作クラス
 * @author miyahara046
 */

#include "BackgroundPlaCameraAction.h"

using namespace std;

BackgroundPlaCameraAction::BackgroundPlaCameraAction(Robot& _robot, bool _isClockwise,
                                                     int _preTargetAngle, int _postTargetAngle,
                                                     double _targetRotationSpeed, double _threshold,
                                                     double _minArea, const cv::Rect _roi,
                                                     int _position)
  : CompositeMotion(_robot),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle),
    targetRotationSpeed(_targetRotationSpeed),
    threshold(_threshold),
    minArea(_minArea),
    roi(_roi),
    position(_position)
{
}

bool BackgroundPlaCameraAction::isMetPreCondition()
{
  if(position != 0 && robot.getBackgroundDirectionResult().wasDetected
     && robot.getBackgroundDirectionResult().direction
            != static_cast<BackgroundDirection>(position)) {
    cout << "正面の撮影位置ではないため風景の撮影動作は行わない。" << endl;
    return false;
  } else {
    return true;
  }
}

// 判定動作を行う関数
void BackgroundPlaCameraAction::detectDirection(cv::Mat& frame)
{
  cout << "風景の向きの判定開始" << endl;
  BackgroundDirectionDetector detector;
  // 風景の向きを判定
  detector.detect(frame, robot.getBackgroundDirectionResult());
  cout << "風景の向きの判定終了" << endl;
  // 検出結果を取得
  BackgroundDirectionResult& result = robot.getBackgroundDirectionResult();

  // デバッグ出力
  if(!result.wasDetected) {
    cout << "風景が検出されませんでした" << endl;
    return;
  }
  switch(result.direction) {
    case BackgroundDirection::FRONT:
      cout << "風景の向き: FRONT" << endl;
      break;
    case BackgroundDirection::BACK:
      cout << "風景の向き: BACK" << endl;
      break;
    case BackgroundDirection::LEFT:
      cout << "風景の向き: LEFT" << endl;
      break;
    case BackgroundDirection::RIGHT:
      cout << "風景の向き: RIGHT" << endl;
      break;
    default:
      std::cout << "風景の向き: 不明" << std::endl;
      break;
  }
}

void BackgroundPlaCameraAction::run()
{
  if(!isMetPreCondition()) return;

  // 撮影のため回頭
  AngleRotation preRotation(robot, preTargetAngle, targetRotationSpeed, isClockwise);
  preRotation.run();

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  PlaCameraAction plaCameraAction(robot, threshold, minArea, roi);

  cv::Mat frame;

  // 判定用のフレームの獲得
  for(int i = 0; i < 5; ++i) {
    robot.getCameraCaptureInstance().getFrame(frame);
    this_thread::sleep_for(chrono::milliseconds(33));
  }

  // もし初回で正面であればPlaCameraActionを実行、他の方向なら２回目でPlaCameraActionを実行、判定できなければ4回PlaCameraActionを実行
  if(position == 0) {
    // 向きの判定とresultの更新(detection)は1回目(初期位置で)の撮影でしか行わない
    detectDirection(frame);

    // もし判定結果が正面であればアップロード用のプラレール画像を取得する
    if(robot.getBackgroundDirectionResult().wasDetected
       && robot.getBackgroundDirectionResult().direction == BackgroundDirection::FRONT) {
      // FRONT方向の画像を保存
      plaCameraAction.run();
    } else if(!robot.getBackgroundDirectionResult().wasDetected) {
      // 検出結果が未検出の場合は、PlaCameraActionを実行
      cout << "風景向き判定用写真の撮影" << endl;
      plaCameraAction.setImageSaveName("bestframe_" + to_string(position));
      plaCameraAction.run();
    }

  } else if(robot.getBackgroundDirectionResult().wasDetected) {
    // 一回目の撮影で風景が検出されていて、向きがFRONTでなければ、二回目の撮影での風景の向きは確実にFRONTになる。
    cout << "正面での撮影" << endl;
    plaCameraAction.run();
  } else {
    // 一回目検出falseなら、残り、3回の撮影は確定する。
    // 一回目の撮影で風景が検出されていない場合は、残り3つのすべてのpositionで撮影を行い、画像をpositionごとに保存する。
    cout << "風景向き判定用写真の撮影" << endl;
    plaCameraAction.setImageSaveName("bestframe_" + to_string(position));
    plaCameraAction.run();
  }

  // 動作安定のためのスリープ
  this_thread::sleep_for(chrono::milliseconds(10));

  // 黒線復帰のための回頭をする
  AngleRotation postRotation(robot, postTargetAngle, targetRotationSpeed, !isClockwise);
  postRotation.run();
}