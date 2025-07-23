/**
 * @file   PlaCameraAction.cpp
 * @brief  プラレール撮影動作クラス
 * @author miyahara046 HaruArima08
 */
#include "PlaCameraAction.h"
#include <thread>

PlaCameraAction::PlaCameraAction(Robot& _robot, double _threshold, double _minArea,
                                 const cv::Rect& _roi)
  : Motion(_robot), motionDetector(_threshold, _minArea, _roi)
{
}

void PlaCameraAction::run()
{
  // 撮影開始時間を記録
  auto startTime = std::chrono::steady_clock::now();
  // 背景フレームの取得
  getBackgroundFrame();

  // 動体検知中に取得したフレームを格納するベクタの初期化
  std::vector<cv::Mat> capturedFrames;
  capturedFrames.clear();

  // 動体検出結果を格納する構造体
  BoundingBoxDetectionResult detectionResult;

  // 物体の入室から退出までフレームを取得
  while(true) {
    cv::Mat frame;
    robot.getCameraCaptureInstance().getFrame(frame);
    // 約30fpsの速度を想定して33msのウェイトを入れる
    std::this_thread::sleep_for(std::chrono::milliseconds(33));

    // 動体を検出
    motionDetector.detect(frame, detectionResult);

    // 動体検出完了
    if(detectionResult.wasDetected) {
      capturedFrames.push_back(frame.clone());
      break;
    }
  }

  int noMotionCounter = 0;
  // 退出検出
  while(true) {
    cv::Mat frame;
    robot.getCameraCaptureInstance().getFrame(frame);
    // 約30fpsの速度を想定して33msのウェイトを入れる
    std::this_thread::sleep_for(std::chrono::milliseconds(33));

    // 動体を検出
    motionDetector.detect(frame, detectionResult);
    if(detectionResult.wasDetected) {
      // 退出していないとみなしカウントリセット
      noMotionCounter = 0;
    } else {
      // 退出とみなしカウント
      noMotionCounter++;
      // 非検出回数がMAX_NO_MOTIONを超えたら退出とみなし、退出検出完了
      if(noMotionCounter >= MAX_NO_MOTION) {
        break;
      }
    }

    // 検出の有無に関わらずフレームを保存
    capturedFrames.push_back(frame.clone());

    // 約30fpsの速度を想定して33msのウェイトを入れる
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }

  // 背景獲得～退出検出終了までの時間を記録して差を計算
  auto endTime = std::chrono::steady_clock::now();
  auto filmingTime
      = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

  // 撮影時間を出力
  std::cout << "Filiming time: " << filmingTime << std::endl;

  // 撮影されたフレームの中央のフレームをbestFrameに格納
  size_t indexOfFrames = capturedFrames.size() / 2;
  cv::Mat bestFrame = capturedFrames.at(indexOfFrames);

  // bestFrameをJPEGで出力
  FrameSave::save(bestFrame, filePath, uploadFile);
}

void PlaCameraAction::getBackgroundFrame()
{
  cv::Mat firstFrame;
  while(1) {
    // 背景に適しているか判断するフレームの獲得
    // 最初にフレームを取得できない問題解決のため５回目のframeを採用する
    for(int i = 0; i < 5; i++) {
      robot.getCameraCaptureInstance().getFrame(firstFrame);
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    // 動体が検知されるか確認
    cv::Mat secondFrame;
    // 撮影間隔をあけ動体検出の正確性の向上
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    robot.getCameraCaptureInstance().getFrame(secondFrame);
    std::vector<cv::Point> largestContour
        = motionDetector.compareTwoFrames(firstFrame, secondFrame);

    // 動体が検知されなかったら背景画像を撮影できたと判断
    if(largestContour.empty()) {
      break;
    }
  }

  // 動体検出されなかったfirstFrameを背景として登録
  motionDetector.setBackground(firstFrame);
}