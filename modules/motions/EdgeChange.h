/**
 * @file   EdgeChange.h
 * @brief  エッジ切り替え
 * @author miyahara046
 */

#ifndef EDGE_CHANGE_H
#define EDGE_CHANGE_H

#include "Motion.h"

class EdgeChange : public Motion {
 public:
  /**
   * @brief エッジを切り替えるクラス
   */
  EdgeChange(Robot& _robot, const bool& _isLeftEdge) : Motion(_robot), isLeftEdge(_isLeftEdge) {}

  /**
   * @brief エッジを切り替える
   */
  void run() override;

 private:
  const bool& isLeftEdge;  // 切り替え後のエッジ(true:左エッジ, false:右エッジ)
};

#endif