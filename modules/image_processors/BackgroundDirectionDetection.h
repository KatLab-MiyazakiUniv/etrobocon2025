/**
 * @file   BackgroundDirectionDetection.h
 * @brief  風景向き判定動作を行うクラス
 * @author Hara1274
 */

#ifndef BACKGROUND_DIRECTION_DETECTION_H
#define BACKGROUND_DIRECTION_DETECTION_H

#include "Motion.h"
#include "BackgroundDirectionDetector.h"

class BackgroundDirectionDetection {
 public:
  /**
   * コンストラクタ
   * @param _robot                          ロボット制御クラスへの参照
   * @param _backgroundDirectionResult      判定結果への参照
   * @param _backgroundDirectionDetector    風景向き判定器クラスへの参照
   */
  BackgroundDirectionDetection(Robot& _robot, BackgroundDirectionResult& _backgroundDirectionResult,
                               BackgroundDirectionDetector& _backgroundDirectionDetector);

  /**
   * @brief 風景向き判定動作を行う
   */
  void run();

 private:
  BackgroundDirectionResult& result;
  BackgroundDirectionDetector& detector;
};

#endif  // BACKGROUND_DIRECTION_DETECTION_H