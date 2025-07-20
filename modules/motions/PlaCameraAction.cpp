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
    // 背景に適しているか判断するフレームの獲得
    for(int counter = 0; counter < 5; counter++) {
      robot.getCameraCaptureInstance().getFrame(frame);
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    // 動体が検知されるか確認
    cv::Mat compareFrame;
    robot.getCameraCaptureInstance().getFrame(compareFrame);
    std::vector<cv::Point> largestContour = motionDetector.compareTwoFrames(frame, compareFrame);

    // 動体が検知されなかったらbreak
    if(largestContour.empty()) {
      break;
    }
  }

  // 最後のframeを背景として登録
  motionDetector.setBackground(frame);
}

void PlaCameraAction::run()
{
  // 背景フレームの取得
  getBackgroundFrame();

  // 動体検知中に取得したフレームを格納するベクタの初期化
  capturedFrames.clear();

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

      // 非検出カウンタをリセット
      noMotionCounter = 0;
    }
    // 動体が検出されなかったが、すでに一度入室していた場合
    else if(objectEntered) {
      // 非検出の回数をカウント
      noMotionCounter++;

      // 非検出が連続でMAX_NO_MOTIONを超えたら退出とみなしてループ終了
      if(noMotionCounter >= MAX_NO_MOTION) {
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

  // 撮影されたフレームの中央のフレームをbestFrameに格納
  halfNumberOfFrames = numberOfFrames / 2;
  cv::Mat bestFrame = capturedFrames.at(halfNumberOfFrames);

  // bestFrameをJPEGで出力
  std::string outputPath = "etrobocon2025/datafiles/snapshots/bestframe.JPEG";
  if(cv::imwrite(outputPath, bestFrame)) {
    std::cout << "Center frame saved  " << std::endl;
  } else {
    std::cerr << "Failed to save" << std::endl;
  }
}
