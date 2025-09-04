/**
 * @file   BackgroundPlaCameraAction.h
 * @brief  風景・プラレール撮影動作クラス
 * @author miyahara046
 */

#ifndef BACKGROUND_PLACAMERA_ACTION_H
#define BACKGROUND_PLACAMERA_ACTION_H

#include "PlaCameraAction.h"
#include "AngleRotation.h"
#include "Robot.h"
#include "FrameSave.h"
#include "ImageUploader.h"
#include <thread>
#include <iostream>
#include <nlohmann/json.hpp>
#include "CompositeMotion.h"

class BackgroundPlaCameraAction : public CompositeMotion {
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
   * @param _roi 動体検出用の注目領域
   * @param _position 撮影位置（0:正面, 1:右, 2:後ろ, 3:左）
   */
  BackgroundPlaCameraAction(Robot& _robot, bool _isClockwise, int _preTargetAngle,
                            int _postTargetAngle, double _targetRotationSpeed, double _threshold,
                            double _minArea, const cv::Rect roi, int _position);

  /**
   * @brief 撮影動作を実行する
   */
  void run() override;

 private:
  bool isClockwise = false;            // 回頭方向
  int preTargetAngle = 90;             // カメラを風景に向けるための回頭角度
  int postTargetAngle = 90;            // 黒線復帰のための回頭角度
  double targetRotationSpeed = 200.0;  // 目標回頭速度
  double threshold = 30.0;             // 風景検出のしきい値
  double minArea = 400.0;              // 最小面積
  int position = 0;                    // 撮影位置（0:正面, 1:右, 2:後ろ, 3:左）
  cv::Rect roi;                        // 動体検出用の注目領域
  const std::string detectionTargetPath
      = "etrobocon2025/datafiles/detection_target";  // 判定用画像ディレクトリのパス
  const std::string detectionTargetName = "background";  // 風景向き判定用画像ファイル名
  const std::string command
      = "make -C etrobocon2025/yolo_detection_system/ background-detection";  // 風景推論用コマンド
  const std::string resultFilePath
      = "etrobocon2025/yolo_detection_system/result/"
        "background_result.json";  // 風景の向きを判定した結果が出力されるファイルパス

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
};

#endif
