/**
 * @file Calibrator.h
 * @brief キャリブレーションからスタートまでを担当するクラス
 * @author HaruArima08 nishijima515
 */

#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "Robot.h"

#define LEFT 'L'
#define RIGHT 'R'
#define PRESS_POWER 0.5f

class Calibrator {
 public:
  /**
   * コンストラクタ
   */
  Calibrator(Robot& _robot);

  /**
   * @brief キャリブレーション処理（入力系）をまとめて実行する
   */
  void run();

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

 protected:
  Robot& robot;  // Robotインスタンスの参照

 private:
  bool isLeftCourse;     // true:Lコース, false: Rコース
  bool isLeftEdge;       // true:左エッジ, false: 右エッジ
  int targetBrightness;  // 目標輝度

  /**
   * @brief 左右ボタンでLRコースを選択してisLeftCourseをセットする
   */
  void selectAndSetCourse();

  /**
   * @brief 黒と白の輝度を測定して目標輝度を求めtargetBrightnessをセットする
   */
  void measureAndSetTargetBrightness();
};

#endif