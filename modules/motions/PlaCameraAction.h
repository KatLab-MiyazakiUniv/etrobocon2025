/**
 * @file   PlaCameraAction.h
 * @brief  プラレール撮影動作クラス
 * @author miyahara046 HaruArima08
 */

#ifndef PLA_CAMERA_ACTION_H
#define PLA_CAMERA_ACTION_H

#include "SystemInfo.h"
#include "Motion.h"
#include "MotionDetector.h"
#include "FrameSave.h"
#include <vector>
#include <memory>

class PlaCameraAction : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボット本体への参照
   * @param _threshold 差分二値化の閾値
   * @param _minArea 動体とみなす最小面積
   * @param _roi 検出対象領域
   */
  PlaCameraAction(Robot& _robot, double _threshold, double _minArea, const cv::Rect& _roi);

  /**
   * @brief プラレール撮影動作を実行
   */
  void run() override;

 private:
  /**
   * @brief 背景画像の取得
   */
  void getBackgroundFrame();

  MotionDetector motionDetector;
  static constexpr int MAX_NO_MOTION = 2;  // 非検知となる検知回数
  static constexpr const char* filePath = "etrobocon2025/datafiles/plarail/";
  static constexpr const char* uploadFile = "bestframe.JPEG";
};

#endif
