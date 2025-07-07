/**
 * @file   MiniFigCameraAction.h
 * @brief  配置エリアAにおけるミニフィグ撮影動作
 * @author nishijima515
 */

#ifndef MINIFIGCAMERAACTION_H
#define MINIFIGCAMERAACTION_H

#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "MiniFigDirectionDetection.h"

class MiniFigCameraAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _isClockwise フロントカメラをミニフィグに向けるための回頭方向　true:時計回り,
   * false:反時計回り
   * @param _preTargetAngle フロントカメラをミニフィグに向けるための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   * @param _targetRotationSpeed 撮影前後の回頭のための目標速度
   * @param _targetDistance 撮影前後の移動距離
   * @param _forwardSpeed 撮影後の前進速度の絶対値
   * @param _backSpeed 撮影前の後退速度の絶対値
   * @param position 撮影位置（0が初期位置）
   */
  MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle, int _postTargetAngle,
                      double _targetRotationSpeed, double _targetDistance, double _forwardSpeed,
                      double _backSpeed, int position);

  /**
   * @brief 撮影動作を行う
   */
  void run() override;

 private:
  bool isClockwise;     // リアカメラをミニフィグに向けるための回頭方向
  int preTargetAngle;   // フロントカメラをミニフィグに向けるための回頭角度
  int postTargetAngle;  // 黒線復帰のための目標角度
  int targetRotationSpeed;  // 撮影前後の回頭のための目標速度
  double targetDistance;    // 撮影前後の後退距離
  double forwardSpeed;      // 撮影前の前進速度
  double backSpeed;         // 撮影後の後退速度
  int position;             // 撮影位置（0が初期位置）
};
#endif