/**
 * @file   IMUSetting.h
 * @brief  IMU角度の測定を設定する動作
 * @author hara1274
 */

#ifndef IMU_SETTING_H
#define IMU_SETTING_H

#include "Motion.h"

class IMUSetting : public Motion {
 public:
  /**
   * @brief コンストラクタ
   * @param _robot ロボット制御クラスへの参照
   * @param _setting 測定を開始する停止するかの設定 (true:開始, false:停止)
   */
  IMUSetting(Robot& _robot, bool _setting);

  /**
   * @brief IMU角度測定を開始または停止する
   */
  void run() override;

 private:
  bool setting;
};

#endif  // IMU_SETTING_H
