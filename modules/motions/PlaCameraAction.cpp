/**
 * @file   PlaCameraAction.cpp
 * @brief  動体検知用動作クラス
 * @author miyahara046
 */
#include "PlaCameraAction.h"
#include <thread>

PlaCameraAction::PlaCameraAction(Robot& _robot, double _threshold, double _minArea,
                                 const cv::Rect& _roi)
  : Motion(_robot), motionDetector(_threshold, _minArea, _roi)
{
}

void PlaCameraAction::getBackgroundFrame()
{
  cv::Mat frame;
  while(1) {
    for(int counter = 0; counter < 5; counter++) {
      robot.getCameraCaptureInstance().getFrame(frame);
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    cv::Mat compareFrame;
    robot.getCameraCaptureInstance().getFrame(compareFrame);
    std::vector<cv::Point> largestContour = motionDetector.compareTwoFrames(frame, compareFrame);
    if(largestContour.empty()) {
      break;
    }
  }

  // 最後のframeを背景として登録
  motionDetector.setBackground(frame);
}

void PlaCameraAction::run()
{
  // 動体検知中に取得したフレームを格納するベクタ
  getBackgroundFrame();
  capturedFrames.clear();

  // 物体がROIに入ったかどうかを示すフラグ
  bool objectEntered = false;

  // 動きが検出されなくなったフレーム数をカウント
  int noMotionCounter = 0;

  // 「物体が退出した」とみなすために必要な連続する未検出フレーム数
  const int maxNoMotionFrames = 2;

  // 動体検出結果を格納する構造体
  BoundingBoxDetectionResult detectionResult;

  // 物体の入室から退出までフレームを取得
  while(1) {
    // カメラからフレームを取得
    cv::Mat frame;
    robot.getCameraCaptureInstance().getFrame(frame);

    // 動体を検出
    motionDetector.detect(frame, detectionResult);

    // 動体が検出された場合（ROIに物体が入っているとみなす）
    if(detectionResult.wasDetected) {
      if(!objectEntered) {
        objectEntered = true;
      }

      // フレームを保存
      capturedFrames.push_back(frame.clone());

      // 非検出カウンタをリセット（動きがあるので）
      noMotionCounter = 0;
    }
    // 動体が検出されなかったが、すでに一度入室していた場合
    else if(objectEntered) {
      // 非検出の回数をカウント
      noMotionCounter++;

      // 非検出が連続 maxNoMotionFrames 回を超えたら退出とみなしてループ終了
      if(noMotionCounter >= maxNoMotionFrames) {
        break;
      }

      // 動きがない間もフレームを保存しておく
      capturedFrames.push_back(frame.clone());
    }

    // 約30fpsの速度を想定して33msのウェイトを入れる
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  // 撮影されたフレームの数を出力
  numberOfFrames = capturedFrames.size();
  std::cout << "Captured frames: " << numberOfFrames << std::endl;

  halfNumberOfFrames = numberOfFrames / 2;
  cv::Mat bestFrame = capturedFrames.at(halfNumberOfFrames);

  std::string outputPath = "etrobocon2025/datafiles/snapshots/bestframe.JPEG";
  if(cv::imwrite(outputPath, bestFrame)) {
    std::cout << "Center frame saved  " << std::endl;
  } else {
    std::cerr << "Failed to save" << std::endl;
  }
}
