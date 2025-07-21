/**
 * @file   PlaCameraAction.h
 * @brief  動体検知用動作クラス
 * @author miyahara046 HaruArima08
 */

#ifndef PLA_CAMERA_ACTION_H
#define PLA_CAMERA_ACTION_H

#include "SystemInfo.h"
#include "Motion.h"
#include "MotionDetector.h"
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
   * @brief 背景画像の取得
   */
  void getBackgroundFrame();

  /**
   * @brief 動体検知動作を実行
   */
  void run() override;

 private:
  MotionDetector motionDetector;
  std::vector<cv::Mat> capturedFrames;     // 動体検知中に取得したフレームを格納するベクタ
  size_t numberOfFrames;                   // 取得した総フレーム数
  size_t halfNumberOfFrames;               // 取得した総フレームの半数
  bool objectEntered = false;              // 物体がROIに入ったかどうかを示すフラグ
  static constexpr int MAX_NO_MOTION = 2;  // 非検知となる検知回数
  int noMotionCounter = 0;                 // 動きが検出されなくなったフレーム数をカウント
};

#endif
