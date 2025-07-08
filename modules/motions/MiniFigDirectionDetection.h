/**
 * @file   MiniFigDirectionDetection.h
 * @brief  ミニフィグの判定動作クラス
 * @author nishijima515
 */

#ifndef MINIFIG_DIRECTION_DETECTION_H
#define MINIFIG_DIRECTION_DETECTION_H

#include "MiniFigDirectionDetector.h"
#include "Motion.h"
#include "FrameSave.h"

class MiniFigDirectionDetection : public Motion {
 public:
  MiniFigDirectionDetection(Robot& _robot, cv::Mat& _frame);

  void run();

 private:
  cv::Mat& frame;  // カメラから取得した画像フレーム
};

#endif  // MINIFIG_DIRECTION_DETECTION_H