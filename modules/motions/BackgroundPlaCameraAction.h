#ifndef BACKGROUND_PLA_CAMERA_ACTION_H
#define BACKGROUND_PLA_CAMERA_ACTION_H

#include "IMUAngleRotation.h"
#include "IMUDistanceStraight.h"
#include "CompositeMotion.h"
#include <opencv2/opencv.hpp>  // For cv::Rect

class BackgroundPlaCameraAction : public CompositeMotion {
 public:
  void run() override;

  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _isClockwise 時計回りかどうか
   * @param _preTargetAngle カメラを風景に向けるための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   * @param _basePower 回頭基準パワー値
   * @param _targetDistance 直進距離 [mm]
   * @param _preTargetSpeed 直進目標速度 [mm/s]
   * @param _postTargetSpeed 後退目標速度 [mm/s]
   * @param _threshold 風景検出のしきい値
   * @param _minArea 最小面積
   * @param _roi 動体検出用の注目領域
   * @param _position 撮影位置（0:正面, 1:右, 2:後ろ, 3:左）
   * @param _isAbsoluteAngleMode 回頭方法 false:相対角度回頭, true:絶対角度回頭
   * @param _kp 回頭PIDのP値
   * @param _ki 回頭PIDのI値
   * @param _kd 回頭PIDのD値
   */
  BackgroundPlaCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                            int _postTargetAngle, int _basePower, double _targetDistance, double _preTargetSpeed, double _postTargetSpeed, double _threshold,
                            double _minArea, const cv::Rect roi, int _position,
                            bool _isAbsoluteAngleMode, double _kp = 0.036, double _ki = 0.02,
                            double _kd = 0.03);

 private:
  bool isClockwise = false;          // 回頭方向
  int preTargetAngle = 90;           // カメラを風景に向けるための回頭角度
  int postTargetAngle = 90;          // 黒線復帰のための回頭角度
  int basePower = 50;                // 回頭基準パワー値
  double targetDistance = 100.0; // 直進距離 [mm]
  double preTargetSpeed = 400.0; //直進目標速度
  double postTargetSpeed = -400.0; //後退目標速度
  double threshold = 30.0;           // 風景検出のしきい値
  double minArea = 400.0;            // 最小面積
  int position = 0;                  // 撮影位置（0:正面, 1:右, 2:後ろ, 3:左）
  cv::Rect roi;                      // 動体検出用の注目領域
  bool isAbsoluteAngleMode = false;  // 回頭方法 false:相対角度回頭, true:絶対角度回頭
  double kp = 0.036;                 // 回頭PIDのP値
  double ki = 0.02;                  // 回頭PIDのI値
  double kd = 0.03;                  // 回頭PIDのD値

  /**
   * @brief 前提条件を満たしているかチェックする
   * @return 前提条件を満たしている場合はtrue、そうでない場合はfalse
   */
  bool isMetPreCondition();
};

#endif