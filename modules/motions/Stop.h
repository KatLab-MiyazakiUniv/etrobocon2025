/**
 * @file   Stop.h
 * @brief  走行体を停止させる動作
 * @author Hara1274 HaruArima08
 */

#ifndef STOP_H
#define STOP_H

#include "Motion.h"

class Stop : public Motion {
 public:
  Stop(Robot& _robot);

  /**
   * @brief モータを停止する
   */
  void run() override;
};

#endif  // STOP_H