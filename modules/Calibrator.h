/**
 * @file Calibrator.h
 * @brief キャリブレーションからスタートまでを担当するクラス
 * @author HaruArima08 nishijima515
 */

#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "Robot.h"
#include "Snapshot.h"

#define PRESS_POWER 0.5f

class Calibrator {
 public:
  /**
   * コンストラクタ
   */
  Calibrator(Robot& _robot);

  /**
   * @brief 左右ボタンでLRコースを選択してisLeftCourseをセットする
   */
  void selectAndSetCourse();

  /**
   * @brief 黒と白の輝度を測定して目標輝度を求めtargetBrightnessをセットする
   */
  void measureAndSetTargetBrightness();

  /**
   * @brief カメラ角度調整用のフレーム取得をする
   */
  void getAngleCheckFrame();

  /**
   * @brief スタート合図が出るまで待機状態にする
   */
  void waitForStart();

  /**
   * @brief isLeftCourseのゲッター
   * @return true:Lコース, false:Rコース
   */
  bool getIsLeftCourse();

  /**
   * @brief targetBrightnessのゲッター
   * @return 目標輝度
   */
  int getTargetBrightness();

 private:
  Robot& robot;          // Robotインスタンスの参照
  bool isLeftCourse;     // true:Lコース, false: Rコース
  int targetBrightness;  // 目標輝度
};

#endif