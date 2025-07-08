/**
 * @file   MiniFigDirectionDetection.cpp
 * @brief  ミニフィグの判定動作クラス
 * @author nishijima515
 */

#include "MiniFigDirectionDetection.h"

MiniFigDirectionDetection::MiniFigDirectionDetection(Robot& _robot, cv::Mat& _frame)
  : Motion(_robot), frame(_frame)
{
}

/**
 * ミニフィグの向き判定動作を実行する
 */
void MiniFigDirectionDetection::run()
{
  MiniFigDirectionDetector detector;
  // ミニフィグの向きを判定
  detector.detect(frame, robot.getMiniFigDirectionResult());

  // 検出結果を取得
  MiniFigDirectionResult& result = robot.getMiniFigDirectionResult();

  // デバッグ出力
  if(result.wasDetected) {
    switch(result.direction) {
      case MiniFigDirection::FRONT:
        printf("ミニフィグの向き: FRONT\n");
        break;
      case MiniFigDirection::BACK:
        printf("ミニフィグの向き: BACK\n");
        break;
      case MiniFigDirection::LEFT:
        printf("ミニフィグの向き: LEFT\n");
        break;
      case MiniFigDirection::RIGHT:
        printf("ミニフィグの向き: RIGHT\n");
        break;
    }
  } else {
    printf("ミニフィグが検出されませんでした\n");
  }
}