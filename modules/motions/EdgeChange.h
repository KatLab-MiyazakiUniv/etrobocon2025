/**
 * @file   EdgeChange.h
 * @brief  エッジ切り替え
 * @author miyahara046
 */

#ifndef EDGE_CHANGE
#define EDGE_CHANGE

#include "Robot.h"

class EdgeChange {
 public:
  /**
   * コンストラクタ
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   * @param _nextEdge 切り替え後のエッジ(true:左エッジ, false:右エッジ)
   */
  EdgeChange(bool& _isLeftEdge, bool _nextEdge);

  /**
   * @brief エッジを切り替える
   */
  void run();

  /**
   * @brief 現在のエッジの状態を取得
   * @return true: 左エッジ, false: 右エッジ
   */
  bool getIsLeftEdge() const;

 private:
  bool& isLeftEdge;  // 参照によって外部の isLeftEdge を操作
  bool nextEdge;
};

#endif