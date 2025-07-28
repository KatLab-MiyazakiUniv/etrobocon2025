/**
 * @file   BackgroundCameraAction.h
 * @brief  風景撮影動作クラス
 * @author miyahara046
 */

#ifndef BACKGROUND_CAMERA_ACTION_H
#define BACKGROUND_CAMERA_ACTION_H

#include "PlaCameraAction.h"
#include "BackgroundDirectionDetector.h"
#include "AngleRotation.h"
#include "Robot.h"
#include "FrameSave.h"
#include <thread>
#include <iostream>
#include "CompositeMotion.h"

class BackgroundCameraAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _isClockwise 時計回りかどうか
   * @param _preTargetAngle カメラを風景に向けるための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   * @param _targetRotationSpeed 目標回頭速度
   * @param _threshold 風景検出のしきい値
   * @param _minArea 最小面積
   * @param _roi ROI領域
   * @param _position 撮影位置（0:正面, 1:左, 2:右, 3:後ろ）
   */
  BackgroundCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                         int _postTargetAngle, double _targetRotationSpeed, double _threshold,
                         double _minArea, const cv::Rect& _roi, int _position);

  /**
   * @brief 撮影動作を実行する
   */
  void run();

 private:
  /**
   * @brief 前提条件を満たしているかチェックする
   * @return 前提条件を満たしている場合はtrue、そうでない場合はfalse
   */
  bool isMetPreCondition();
  /**
   * @brief 風景の向きを判定する
   * @param frame 入力画像フレーム
   */
  void detectDirection(cv::Mat& frame);

  bool isClockwise = false;          // 回頭方向
  int preTargetAngle = 90;           // カメラを風景に向けるための回頭角度
  int postTargetAngle = 90;          // 黒線復帰のための回頭角度
  double targetRotationSpeed = 200;  // 目標回頭速度
  double threshold = 30.0;           // 風景検出のしきい値
  double minArea = 400.0;            // 最小面積
  const cv::Rect& roi;               // ROI領域
  BackgroundDirectionDetector detector;
  int position = 0;  // 撮影位置（0:正面, 1:左, 2:右, 3:後ろ）
};

#endif
