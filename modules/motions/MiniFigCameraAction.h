#ifndef MINIFIG_CAMERA_ACTION_H
#define MINIFIG_CAMERA_ACTION_H

#include "CompositeMotion.h"
#include "IMUAngleRotation.h"
#include "DistanceStraight.h"

class MiniFigCameraAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _isClockwise
   * カメラをミニフィグに向けるための回頭方向　true:時計回り, false:反時計回り
   * @param _preTargetAngle カメラをミニフィグに向けるための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   * @param _basePower 撮影前後の回頭のための基準パワー値
   * @param _backTargetDistance 撮影前の後退距離
   * @param _forwardTargetDistance 撮影後の前進距離
   * @param _backSpeed 撮影前の後退速度の絶対値
   * @param _forwardSpeed 撮影後の前進速度の絶対値
   * @param _position 撮影位置（0が1回目の撮影箇所）反時計回りに3まで
   * @param _isAbsoluteAngleMode 回頭方法 false:相対角度回頭, true:絶対角度回頭
   * @param _kp 回頭PIDのP値
   * @param _ki 回頭PIDのI値
   * @param _kd 回頭PIDのD値
   */
  MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle, int _postTargetAngle,
                      int _basePower, double _backTargetDistance, double _forwardTargetDistance,
                      double _backSpeed, double _forwardSpeed, int _position, bool _isAbsoluteAngleMode,
                      double _kp = 0.036, double _ki = 0.02, double _kd = 0.03);

  /**
   * @brief ミニフィグの向きを判定し、必要なら撮影動作をスキップする準備処理
   */
  void run() override;

 private:
  bool isClockwise = false;         // カメラをミニフィグに向けるための回頭方向
  int preTargetAngle = 90;          // カメラをミニフィグに向けるための回頭角度
  int postTargetAngle = 90;         // 黒線復帰のための目標角度
  int basePower = 50;               // 撮影前後の回頭のための基準パワー値
  double backTargetDistance = 150;  // 撮影前の後退距離
  double forwardTargetDistance = 150;  // 撮影後の前進距離
  double backSpeed = 200;              // 撮影後の後退速度
  double forwardSpeed = 200;           // 撮影前の前進速度
  int position = 0;  // 撮影位置（0が1回目の撮影箇所）反時計回りに3まで
  bool isAbsoluteAngleMode = false;  // 回頭方法 false:相対角度回頭, true:絶対角度回頭
  double kp = 0.036;            // 回頭PIDのP値
  double ki = 0.02;             // 回頭PIDのI値
  double kd = 0.03;             // 回頭PIDのD値

  /**
   * @brief ミニフィグ撮影動作をする際の事前条件判定をする
   * @return true: 事前条件を満たす,false: 事前条件を満たさない
   */
  bool isMetPreCondition();
};

#endif