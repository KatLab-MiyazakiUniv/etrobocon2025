/**
 * @file   MiniFigCameraAction.h
 * @brief  配置エリアAにおけるミニフィグ撮影動作
 * @author nishijima515
 */

#ifndef MINI_FIG_CAMERA_ACTION_H
#define MINI_FIG_CAMERA_ACTION_H

#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "MiniFigDirectionDetector.h"
#include "FrameSave.h"

class MiniFigCameraAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _isClockwise
   * カメラをミニフィグに向けるための回頭方向　true:時計回り, false:反時計回り
   * @param _preTargetAngle カメラをミニフィグに向けるための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   * @param _targetRotationSpeed 撮影前後の回頭のための目標速度
   * @param _backTargetDistance 撮影前の後退距離
   * @param _forwardTargetDistance 撮影後の前進距離
   * @param _backSpeed 撮影前の後退速度の絶対値
   * @param _forwardSpeed 撮影後の前進速度の絶対値
   * @param _position 撮影位置（0が1回目の撮影箇所）反時計回りに3まで
   */
  MiniFigCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle, int _postTargetAngle,
                      double _targetRotationSpeed, double _backTargetDistance,
                      double _forwardTargetDistance, double _backSpeed, double _forwardSpeed,
                      int _position);

  /**
   * @brief ミニフィグの向きを判定し、必要なら撮影動作をスキップする準備処理
   */
  void run() override;

 private:
  bool isClockwise = false;            // カメラをミニフィグに向けるための回頭方向
  int preTargetAngle = 90;             // カメラをミニフィグに向けるための回頭角度
  int postTargetAngle = 90;            // 黒線復帰のための目標角度
  double targetRotationSpeed = 200;    // 撮影前後の回頭のための目標速度
  double backTargetDistance = 150;     // 撮影前の後退距離
  double forwardTargetDistance = 150;  // 撮影後の前進距離
  double backSpeed = 200;              // 撮影後の後退速度
  double forwardSpeed = 200;           // 撮影前の前進速度
  int position = 0;                    // 撮影位置（0が1回目の撮影箇所）反時計回りに3まで
  const std::string detectionTargetPath
      = "etrobocon2025/datafiles/detection_target";  // 判定用画像ディレクトリのパス
  const std::string detectionTargetName = "fig";     // ミニフィグ向き判定用画像ファイル名
  static constexpr const char* filePath
      = "etrobocon2025/datafiles/figures/";  // ミニフィグ画像保存先のディレクトリパス
  static constexpr const char* uploadFileName = "upload_front_fig";  // アップロード用の画像名

  /**
   * @brief ミニフィグ撮影動作をする際の事前条件判定をする
   * @return true: 事前条件を満たす,false: 事前条件を満たさない
   */
  bool isMetPreCondition();

  /**
   * @brief 判定動作を行う
   * @param frame 処理対象の画像フレーム
   */
  void detectDirection(cv::Mat& frame);
};
#endif